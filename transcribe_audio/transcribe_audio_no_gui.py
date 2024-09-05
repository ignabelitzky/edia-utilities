import os
import sys
import whisper
import ffmpeg

def convert_to_wav(mp3_file):
    """Convert the selected MP3 file to WAV format."""
    wav_file = os.path.splitext(mp3_file)[0] + ".wav"
    ffmpeg.input(mp3_file).output(wav_file, ar='16000').run(overwrite_output=True)
    return wav_file

def transcribe_audio(wav_file, save_path, model_name="large"):
    """Transcribe the audio using Whisper and save it to the specified location."""
    model = whisper.load_model(model_name)
    result = model.transcribe(wav_file, language="es")
    with open(save_path, "w") as file:
        file.write(result['text'])

def main(mp3_file):
    # Convert MP3 to WAV
    wav_file = convert_to_wav(mp3_file)

    # Define the output save path
    save_path = os.path.splitext(mp3_file)[0] + ".txt"
    
    # Transcribe the audio and save it
    transcribe_audio(wav_file, save_path)
    print(f"Transcription saved to {save_path}")

if __name__ == "__main__":
    print("Program to transcribe audio from an MP3 file.\n")
    if len(sys.argv) != 2:
        print("Usage: python script.py <path_to_mp3_file>")
        sys.exit(1)
    
    mp3_file = sys.argv[1]
    if not os.path.isfile(mp3_file):
        print(f"File not found: {mp3_file}.\nPlease provide a valid path to an MP3 file.")
        sys.exit(1)
    
    main(mp3_file)
