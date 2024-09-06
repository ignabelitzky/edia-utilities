import tkinter as tk
from tkinter import filedialog

def select_file():
    """Open a file dialog to select an MP3 file."""
    file_path = filedialog.askopenfilename(
        title = "Select an MP3 file",
        initialdir="~/",
        filetypes = (("MP3 files", "*.mp3"), ("All files", "*.*"))
    )
    return file_path

def save_file():
    """Open a file dialog to select the save location."""
    save_path = filedialog.asksaveasfilename(
        title = "Save Transcription As",
        initialdir="~/",
        defaultextension = ".txt",
        filetypes = (("Text files", "*.txt"), ("All files", "*.*"))
    )
    return save_path