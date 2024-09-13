import tkinter as tk
from tkinter import filedialog
import os


def select_mp3_file():
    """Open a file dialog to select an MP3 file and return the file path."""
    file_path = filedialog.askopenfilename(
        title="Select an MP3 file",
        initialdir=os.path.expanduser("~/"),
        filetypes=(("MP3 files", "*.mp3"), ("All files", "*.*"))
    )
    return file_path


def save_file():
    """Open a file dialog to select the save location."""
    save_path = filedialog.asksaveasfilename(
        title="Save Transcription As",
        initialdir=os.path.expanduser("~/"),
        defaultextension=".txt",
        filetypes=(("Text files", "*.txt"), ("All files", "*.*"))
    )
    return save_path
