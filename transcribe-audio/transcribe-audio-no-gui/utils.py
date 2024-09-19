def format_timestamp(seconds):
    hours = int(seconds // 3600)
    minutes = int((seconds % 3600) // 60)
    seconds = int(seconds % 60)
    return f"{hours:02d}:{minutes:02d}:{seconds:02d}"

def select_wav_file():
    file_path = input("Enter the path to the WAV file: ")
    return file_path

def save_file():
    save_path = input("Enter the path to save the transcription: ")
    return save_path