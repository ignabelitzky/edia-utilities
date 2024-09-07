import os
import tkinter as tk
from tkinter import messagebox
from file_dialogs import select_file, save_file
from conversion import convert_mp3_to_wav
from options import choose_model, choose_language
from transcription import transcribe_audio


def main():
    # Hide the main Tkinter window
    root = tk.Tk()
    root.withdraw()

    # Select the MP3 file
    mp3_file = select_file()
    if not mp3_file:
        messagebox.showinfo("No file selected",
                            "No MP3 file was selected.")
        return

    # Convert MP3 to WAV
    wav_file = convert_mp3_to_wav(mp3_file)
    if not wav_file:
        messagebox.showinfo("Conversion failed",
                            "MP3 to WAV conversion failed.")
        return

    # Choose Whisper model
    model_name = choose_model()
    if not model_name:
        messagebox.showinfo("No model selected",
                            "No Whisper model was selected.")
        return

    # Choose transcription language
    language = choose_language()
    if not language:
        messagebox.showinfo("No language selected",
                            "No language was selected.")
        return

    # Select save location
    save_path = save_file()
    if not save_file:
        messagebox.showinfo("No save location",
                            "No save location was selected.")
        return

    # Transcribe the audio and save it
    try:
        transcribe_audio(wav_file, save_path, model_name, language)
    except Exception as e:
        messagebox.showinfo("Transcription failed", f"Transcription failed: {e}")
        return
    
    messagebox.showinfo("Success", f"Transcription saved to {save_path}")

    # Remove the temporary WAV file
    if os.path.exists(wav_file):
        os.remove(wav_file)
        print(f"Temporary WAV file {wav_file} removed.")


if __name__ == "__main__":
    main()
