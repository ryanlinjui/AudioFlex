import argparse

import librosa
import numpy as np
import soundfile as sf

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Phase vocoder for time-scale and pitch modification"
    )
    parser.add_argument("input_wav_file", type=str, help="Path to the input audio file")
    parser.add_argument(
        "output_wav_file", type=str, help="Path to save the processed audio file"
    )
    parser.add_argument(
        "time_speed",
        type=float,
        help="Time scaling factor (e.g., 1.2 for 1.2x speed-up, 0.8 for 0.8x speed-down)",
    )
    parser.add_argument(
        "pitch_shift",
        type=float,
        help="Pitch shifting factor (e.g., 1.2 for 1.2x pitch shift)",
    )

    args = parser.parse_args()

    y, sr = librosa.load(args.input_wav_file, sr=None)

    if args.pitch_shift != 1.0:
        y = librosa.effects.pitch_shift(y, sr=sr, n_steps=np.log2(args.pitch_shift) * 12)

    hop_length = 512
    X = librosa.stft(y, hop_length=hop_length)

    if args.time_speed != 1.0:
        speed = args.time_speed
        X = librosa.phase_vocoder(X, rate=speed)

    y_out = librosa.istft(X, hop_length=hop_length)
    sf.write(args.output_wav_file, y_out, sr)