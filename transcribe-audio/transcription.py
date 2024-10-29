import whisper

model_cache = {}

def format_timestamp(seconds):
    """Format the timestamp in HH:MM:SS format."""
    hours = int(seconds // 3600)
    minutes = int((seconds % 3600) // 60)
    seconds = int(seconds % 60)
    return f"{hours:02d}:{minutes:02d}:{seconds:02d}"


def transcribe_audio(wav_file, save_path, model_name, language):
    """Transcribe the audio using Whisper and save it with formatted timestamps."""
    try:
        if model_name not in model_cache:
            model_cache[model_name] = whisper.load_model(model_name)
        model = model_cache[model_name]
    except Exception as e:
        raise RuntimeError(f"Failed to load Whisper model: {e}")
    
    try:
        result = model.transcribe(wav_file, language=language, verbose=False)
    except Exception as e:
        raise RuntimeError(f"Failed to transcribe audio: {e}")

    # Save the transcription with timestamps
    with open(save_path, "w") as file:
        for segment in result['segments']:
            start = format_timestamp(segment['start'])
            end = format_timestamp(segment['end'])
            text = segment['text'].strip()
            file.write(f"[{start} - {end}] {text}\n")
