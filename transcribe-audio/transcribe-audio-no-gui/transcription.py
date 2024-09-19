from utils import format_timestamp
import whisper

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