# AudioFlex

## Features
- **Time Stretching**: Increase or decrease the length of audio clips without altering the pitch.
- **Pitch Shifting**: Modify the pitch of audio files without changing the duration.
- **File I/O**: Read and write support for `.wav` audio files. (Sample audio files located in the `audio` directory)

## How does AudioFlex work?

#### (i) Change the Time of a Signal without Varying the Frequency

**Functionality**: Time Stretching  
**Purpose**: This functionality allows users to increase or decrease the playback duration of an audio signal without changing its pitch. This is crucial in applications such as adjusting music tracks to fit a specific time slot, creating slow-motion effects in audio without altering pitch, or making spoken content more accessible by slowing it down for clarity.

**How It Works**:
- **Phase Vocoder**: The primary technology behind this feature is the Phase Vocoder, which processes audio in the frequency domain using Fast Fourier Transform (FFT). The phase vocoder breaks the sound into small overlapping segments or frames, applies an FFT to convert each frame from the time domain to the frequency domain, and then modifies the timing of these frames.
- **Time Stretching Algorithm**: In the frequency domain, the algorithm adjusts the time intervals between successive frames without changing their spectral content. This is accomplished by manipulating the phase of the frequency components to ensure coherence in the overlapped windows during synthesis. When the frames are transformed back to the time domain using the Inverse Fast Fourier Transform (IFFT) and recombined, the result is a longer or shorter version of the original sound with the pitch preserved.

#### (ii) Change the Frequency of a Signal without Varying the Time

**Functionality**: Pitch Shifting  
**Purpose**: This functionality allows users to change the pitch or key of an audio signal without affecting its duration. This feature is essential in music production for key modulation and harmony generation, voice modification in post-production for entertainment or privacy purposes, and making music and educational content more versatile.

**How It Works**:
- **Phase Vocoder**: Similar to time stretching, pitch shifting also uses the phase vocoder technology. However, instead of changing the time intervals between the frames, pitch shifting modifies the frequencies within each frame.
- **Pitch Shifting Algorithm**: By multiplying the frequency components obtained from the FFT by a constant factor (related to the desired pitch shift), the frequencies are effectively raised or lowered. For instance, to increase the pitch, frequencies are scaled up, whereas to decrease the pitch, they are scaled down. The phase components must also be appropriately adjusted to maintain a natural sound and prevent artifacts. After the frequency adjustment, the signal is transformed back to the time domain using IFFT, preserving the original duration but with a modified pitch.

## C++ Version Tutorial

> `cd` into the `cpp` directory to access the C++ version of the AudioFlex project.

### Prerequisites
- C++ Compiler (GCC or Clang)
- `libsndfile` library
- `fftw3` library

### Compiling on Linux
1. Install the necessary libraries:
   ```bash
   sudo apt-get install libsndfile1-dev libfftw3-dev
   ```
2. Compile the project:
    - Navigate to the project directory and compile using `g++`:
        ```bash
        g++ -o main main.cpp -lsndfile -lfftw3
        ```
    - Or use the provided `Makefile`:
        ```bash
        make
        ```

### Example Usage

This example would stretch the audio time by 50% and reduce the pitch by 20%.

```bash
./main -i ./audio/input.wav -o output.wav -t 1.5 -p 0.8
```

### Docker Usage

1. Build the Docker image:
   ```bash
   docker build -t audioflex .
   ```

2. Run the Docker container:
   ```bash
    docker run --rm -v $(pwd)/audio:/audio audioflex ./main <input_file> <output_file> <time_stretch_factor> <pitch_shift_factor>
    ```