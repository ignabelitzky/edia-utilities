from transcription import transcribe_audio
from selection import choose_model, choose_language
from utils import select_wav_file, save_file

def main():
    wav_file = select_wav_file()
    if not wav_file:
        print("No WAV file was selected.")
        return
    
    model_name = choose_model()
    if not model_name:
        print("No Whisper model was selected.")
        return
    
    language = choose_language()
    if not language:
        print("No language was selected.")
        return
    
    save_path = save_file()
    if not save_path:
        print("No save location was selected.")
        return
    
    try:
        transcribe_audio(wav_file, save_path, model_name, language)
    except Exception as e:
        print(f"Transcription failed: {e}")
    
    print(f"Transcription saved to {save_path}")

if __name__ == "__main__":
    main()