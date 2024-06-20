import argparse

import librosa
import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Plots the spectrogram of an audio file"
    )
    parser.add_argument("input_wav_file", type=str, help="Path to the input audio file")
    parser.add_argument("output_plot_file", type=str, help="Path to save the plot image")

    args = parser.parse_args()
    
    y, sr = librosa.load(args.input_wav_file, sr=None)

    hop_length = 512
    X = librosa.stft(y, hop_length=hop_length)
    Xdb = librosa.amplitude_to_db(np.abs(X))

    plt.figure(figsize=(30, 7))
    plt.title(args.input_wav_file)
    librosa.display.specshow(Xdb, sr=sr, x_axis="time", y_axis="hz")
    plt.colorbar()
    plt.tight_layout()
    plt.savefig(args.output_plot_file)