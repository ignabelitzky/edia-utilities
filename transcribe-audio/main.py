import os
import tkinter as tk
from tkinter import messagebox
from file_dialogs import select_audio_file, save_file
from conversion import convert_mp3_to_wav
from options import choose_model_base_on_language, choose_language
from transcription import transcribe_audio


def main():
    root = tk.Tk()
    root.withdraw() # Hide the main window

    wav_file = select_audio_file()
    if not wav_file:
        messagebox.showinfo("No file selected", "No Audio file was selected.")
        return

    if not wav_file.lower().endswith('.wav'):
        wav_file = convert_mp3_to_wav(wav_file)
        if not wav_file:
            messagebox.showinfo("Conversion failed", "MP3 to WAV conversion failed.")
            return
    
    language = choose_language()
    if not language:
        messagebox.showinfo("No language selected", "No language was selected.")
        return

    model_name = choose_model_base_on_language(language)
    if not model_name:
        messagebox.showinfo("No model selected", "No Whisper model was selected.")
        return

    save_path = save_file()
    if not save_path:
        messagebox.showinfo("No save location", "No save location was selected.")
        return

    try:
        transcribe_audio(wav_file, save_path, model_name, language)
        messagebox.showinfo("Success", f"Transcription saved to {save_path}")
    except Exception as e:
        messagebox.showinfo("Transcription failed", f"Transcription failed: {e}")
    
    root.destroy()


if __name__ == "__main__":
    main()
