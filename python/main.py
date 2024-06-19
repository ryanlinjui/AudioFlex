import numpy as np
import librosa
import soundfile as sf
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Phase vocoder for time-scale and pitch modification"
    )
    parser.add_argument("input_file", type=str, help="Path to the input audio file")
    parser.add_argument(
        "output_file", type=str, help="Path to save the processed audio file"
    )
    parser.add_argument(
        "time_scale",
        type=float,
        help="Time scaling factor (e.g., 1.5 for 1.5x time scale)",
    )
    parser.add_argument(
        "pitch_shift",
        type=float,
        help="Pitch shifting factor (e.g., 1.2 for 1.2x pitch shift)",
    )

    args = parser.parse_args()

    y, sr = librosa.load(args.input_file, sr=None)

    if args.pitch_shift != 1.0:
        y = librosa.effects.pitch_shift(y, sr=sr, n_steps=np.log2(args.pitch_shift) * 12)

    hop_length = 512
    X = librosa.stft(y, hop_length=hop_length)

    if args.time_scale != 1.0:
        speed = 1.0 / args.time_scale
        X = librosa.phase_vocoder(X, rate=speed)

    y_out = librosa.istft(X, hop_length=hop_length)
    sf.write(args.output_file, y_out, sr)

