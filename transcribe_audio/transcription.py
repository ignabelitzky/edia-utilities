import whisper

def format_timestamp(seconds):
    """Format the timestamp in HH:MM:SS format."""
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