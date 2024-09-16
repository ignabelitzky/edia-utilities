import whisper

def select_wav_file():
    file_path = input("Enter the path to the WAV file: ")
    return file_path

def choose_model():
    print("Available Whisper models:")
    print("1. tiny")
    print("2. small")
    print("3. medium")
    print("4. large")
    model_number = input("Select the number of the model you want to use: ")
    if model_number == "1":
        model_name = "tiny"
    elif model_number == "2":
        model_name = "small"
    elif model_number == "3":
        model_name = "medium"
    elif model_number == "4":
        model_name = "large"
    else:
        model_name = None
    return model_name

def format_timestamp(seconds):
    hours = int(seconds // 3600)
    minutes = int((seconds % 3600) // 60)
    seconds = int(seconds % 60)
    return f"{hours:02d}:{minutes:02d}:{seconds:02d}"

def choose_language():
    print("Available languages:")
    print("1. English")
    print("2. Spanish")
    print("3. French")
    language_number = input("Select the number of the language you want to transcribe: ")
    if language_number == "1":
        language = "en"
    elif language_number == "2":
        language = "es"
    elif language_number == "3":
        language = "fr"
    else:
        language = None
    return language

def save_file():
    save_path = input("Enter the path to save the transcription: ")
    return save_path

def transcribe_audio(wav_file, save_path, model_name, language):
    print(f"Transcribing {wav_file} using the {model_name} model for {language}...")
    try:
        model = whisper.load_model(model_name)
    except Exception as e:
        raise RuntimeError(f"Failed to load Whisper model: {e}")
    
    try:
        result = model.transcribe(wav_file, language=language, verbose=False)
    except Exception as e:
        raise RuntimeError(f"Failed to transcribe audio: {e}")
    
    with open(save_path, "w") as file:
        for segment in result['segments']:
            start = format_timestamp(segment['start'])
            end = format_timestamp(segment['end'])
            text = segment['text'].strip()
            file.write(f"[{start} - {end}] {text}\n")

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