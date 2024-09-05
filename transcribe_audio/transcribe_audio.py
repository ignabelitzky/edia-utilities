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

def choose_model():
    """Prompt the user to choose a Whisper model using a dropdown menu."""
    models = ["tiny", "base", "small", "medium", "large"]

    # Create a new Tkinter window for the dropdown menu
    model_window = tk.Tk()
    model_window.title("Choose Whisper Model")

    # Create a variable for the dropdown menu
    model_var = tk.StringVar(value="medium")

    # Create and place the dropdown menu
    tk.Label(model_window, text="Select a Whisper model").pack(pady=10)
    model_menu = tk.OptionMenu(model_window, model_var, *models)
    model_menu.pack(pady=10)

    # Create a button to confirm the selection
    def confirm_selection():
        model_window.quit()
    
    tk.Button(model_window, text="OK", command=confirm_selection).pack(pady=10)

    # Run the Tkinter main loop to display the window
    model_window.mainloop()

    # Get the selected model value after closing
    selected_model = model_var.get()
    model_window.destroy()
    return selected_model

def choose_language():
    """Prompt the user to choose the transcription language (English or Spanish)."""
    languages = {"English": "en", "Spanish": "es"}

    # Create a new Tkinter window for the dropdown menu
    lang_window = tk.Tk()
    lang_window.title("Choose Language")

    # Create a variable for the dropdown menu
    lang_var = tk.StringVar(value="es")

    # Create and place the dropdown menu
    tk.Label(lang_window, text="Select a transcription language").pack(pady=10)
    lang_menu = tk.OptionMenu(lang_window, lang_var, *languages.keys())
    lang_menu.pack(pady=10)

    # Create a button to confirm the selection
    def confirm_selection():
        lang_window.quit()

    tk.Button(lang_window, text="OK", command=confirm_selection).pack(pady=10)

    # Run the Tkinter main loop to display the window
    lang_window.mainloop()

    # Get the selected languag code after closing
    selected_language = languages[lang_var.get()]
    lang_window.destroy()
    return selected_language

def format_timestamp(seconds):
    """Convert seconds to HH:MM:SS format."""
    hours = int(seconds // 3600)
    minutes = int(seconds // 60)
    seconds = int(seconds % 60)
    return f"{hours:02d}:{minutes:02d}:{seconds:02d}"

def transcribe_audio(wav_file, save_path, model_name, language):
    """Transcribe the audio using Whisper and save it with formatted timestamps."""
    model = whisper.load_model(model_name)
    result = model.transcribe(wav_file, language=language, verbose=False)
    
    # Save the transcription with timestamps
    with open(save_path, "w") as file:
        for segment in result['segments']:
            start = format_timestamp(segment['start'])
            end = format_timestamp(segment['end'])
            text = segment['text'].strip()
            file.write(f"[{start} - {end}] {text}\n")
    
def main():
    # Create a simple GUI for selecting files
    root = tk.Tk()
    root.withdraw() # Hide the main window

    # Select the MP3 file
    mp3_file = select_file()
    if not mp3_file:
        messagebox.showinfo("No file selected", "No MP3 file was selected.")
        return
    
    # Convert MP3 to WAV
    wav_file = convert_to_wav(mp3_file)

    # Choose Whisper model
    model_name = choose_model()
    if not model_name:
        messagebox.showinfo("No model selected", "No Whisper model was selected.")
        return
    
    # Choose transcription language
    language = choose_language()
    if not language:
        messagebox.showinfo("No language selected", "No language was selected.")
        return

    # Select save location
    save_path = save_file()
    if not save_path:
        messagebox.showinfo("No save location", "No save location was selected.")
        return
    
    # Transcribe the audio and save it
    transcribe_audio(wav_file, save_path, model_name, language)
    messagebox.showinfo("Success", f"Transcription saved to {save_path}")
    
    # Remove the temporary WAV file
    if os.path.exists(wav_file):
        os.remove(wav_file)
        print(f"Temporary WAV file {wav_file} removed.")
    
    print(f"Transcription completed and saved to {save_path}")

if __name__ == "__main__":
    main()
