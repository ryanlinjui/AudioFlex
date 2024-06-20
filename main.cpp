#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

#include <sndfile.h>
#include <fftw3.h>

void read_audio(
    const char* input_path,
    std::vector<double>& audio_data,
    int32_t& sample_rate
){
    SF_INFO sfinfo;
    SNDFILE* infile = sf_open(input_path, SFM_READ, &sfinfo);
    if (!infile)
    {
        std::cerr << "Failed to open input file." << std::endl;
        exit(1);
    }

    sample_rate = sfinfo.samplerate;
    audio_data.resize(sfinfo.frames * sfinfo.channels);
    sf_read_double(infile, audio_data.data(), sfinfo.frames * sfinfo.channels);
    sf_close(infile);

    if (sfinfo.channels > 1)
    {
        // Convert stereo to mono by averaging channels
        std::vector<double> mono_audio(sfinfo.frames);
        for (size_t i = 0; i < sfinfo.frames; ++i)
        {
            double sum = 0;
            for (int j = 0; j < sfinfo.channels; ++j)
            {
                sum += audio_data[i * sfinfo.channels + j];
            }
            mono_audio[i] = sum / sfinfo.channels;
        }
        audio_data = mono_audio;
    }
}

void write_audio(
    const char* output_path,
    const std::vector<double>& audio_data,
    int32_t sample_rate
){
    SF_INFO sfinfo;
    sfinfo.frames = audio_data.size();
    sfinfo.samplerate = sample_rate;
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE* outfile = sf_open(output_path, SFM_WRITE, &sfinfo);
    if (!outfile)
    {
        std::cerr << "Failed to open output file." << std::endl;
        exit(1);
    }

    sf_write_double(outfile, audio_data.data(), audio_data.size());
    sf_close(outfile);
}

void phase_vocoder(
    const std::vector<double>& input,
    std::vector<double>& output,
    double time_scaling_factor,
    double pitch_shift_factor,
    int32_t sample_rate
){
    int32_t fft_size = 4096;
    int32_t hop_size = fft_size / 4;
    int32_t num_frames = input.size() / hop_size;

    int32_t output_size = input.size() * time_scaling_factor;
    output.resize(output_size);

    std::vector<double> prev_phase(fft_size);
    std::vector<double> sum_phase(fft_size);
    std::vector<double> window(fft_size);

    for (int i = 0; i < fft_size; ++i)
    {
        window[i] = 0.5 * (1 - cos(2 * M_PI * i / (fft_size - 1))); // Hann window
    }

    fftw_complex *in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_size);
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_size);
    fftw_plan forward_plan = fftw_plan_dft_1d(fft_size, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan inverse_plan = fftw_plan_dft_1d(fft_size, out, in, FFTW_BACKWARD, FFTW_ESTIMATE);

    for (int frame = 0; frame < num_frames - 1; ++frame)
    {
        for (int k = 0; k < fft_size; ++k)
        {
            if (frame * hop_size + k < input.size())
            {
                in[k][0] = input[frame * hop_size + k] * window[k];
            }
            else
            {
                in[k][0] = 0.0;
            }
            
            in[k][1] = 0.0;
        }

        fftw_execute(forward_plan);

        for (int k = 0; k < fft_size; ++k)
        {
            double magnitude = sqrt(out[k][0] * out[k][0] + out[k][1] * out[k][1]);
            double phase = atan2(out[k][1], out[k][0]);

            double delta_phase = phase - prev_phase[k];
            prev_phase[k] = phase;

            delta_phase -= hop_size * 2.0 * M_PI * k / fft_size;
            delta_phase = delta_phase - 2.0 * M_PI * round(delta_phase / (2.0 * M_PI));

            double true_freq = 2.0 * M_PI * k / fft_size + delta_phase / hop_size;

            sum_phase[k] += hop_size * true_freq;

            out[k][0] = magnitude * cos(sum_phase[k] * pitch_shift_factor);
            out[k][1] = magnitude * sin(sum_phase[k] * pitch_shift_factor);
        }

        fftw_execute(inverse_plan);

        for (int k = 0; k < fft_size; ++k)
        {
            if (frame * hop_size * time_scaling_factor + k < output.size())
            {
                output[frame * hop_size * time_scaling_factor + k] += in[k][0] * window[k] / fft_size;
            }
        }
    }

    fftw_destroy_plan(forward_plan);
    fftw_destroy_plan(inverse_plan);
    fftw_free(in);
    fftw_free(out);
}

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <input_path> <output_path> <time_scaling_factor> <pitch_shift_factor>" << std::endl;
        return 1;
    }

    const char* input_path = argv[1];
    const char* output_path = argv[2];
    double time_scaling_factor = 1 / atof(argv[3]);
    double pitch_shift_factor = 1 / atof(argv[4]);

    std::vector<double> audio_data;
    int32_t sample_rate;

    read_audio(input_path, audio_data, sample_rate);
    std::vector<double> output_data;
    phase_vocoder(audio_data, output_data, time_scaling_factor, pitch_shift_factor, sample_rate);
    write_audio(output_path, output_data, sample_rate);

    return 0;
}
