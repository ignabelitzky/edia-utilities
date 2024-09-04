import os
import tkinter as tk
from tkinter import filedialog, messagebox
import whisper
import ffmpeg

def select_file():
    """Open a file dialog to select an MP3 file."""
    file_path = filedialog.askopenfilename(
        title = "Select an MP3 file",
        filetypes = (("MP3 files", "*.mp3"), ("All files", "*.*"))
    )
    return file_path

def save_file():
    """Open a file dialog to select the save location."""
    save_path = filedialog.asksaveasfilename(
        title = "Save Transcription As",
        defaultextension = ".txt",
        filetypes = (("Text files", "*.txt"), ("All files", "*.*"))
    )
    return save_path

def convert_to_wav(mp3_file):
    """Convert the selected MP3 file to WAV format."""
    wav_file = os.path.splitext(mp3_file)[0] + ".wav"
    ffmpeg.input(mp3_file).output(wav_file, ar='16000').run(overwrite_output=True)
    return wav_file

def transcribe_audio(wav_file, save_path):
    """Transcribe the audio using Whisper and save it to the specified location."""
    model = whisper.load_model("base")
    result = model.transcribe(wav_file, language="es")
    with open(save_path, "w") as file:
        file.write(result['text'])

def main():
    # create a simple GUI for selecting files
    root = tk.Tk()
    root.withdraw() # hide the main window

    # Select the MP3 file
    mp3_file = select_file()
    if not mp3_file:
        messagebox.showinfo("No file selected", "No MP3 file was selected.")
        return
    
    # Convert MP3 to WAV
    wav_file = convert_to_wav(mp3_file)

    # Select save location
    save_path = save_file()
    if not save_path:
        messagebox.showinfo("No save location", "No save location was selected.")
        return
    
    # Transcribe the audio and save it
    transcribe_audio(wav_file, save_path)
    messagebox.showinfo("Success", f"Transcription saved to {save_path}")

if __name__ == "__main__":
    main()