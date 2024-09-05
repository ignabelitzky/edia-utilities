import os
import ffmpeg

def convert_mp3_to_wav(mp3_file):
    """Convert an MP3 file to a WAV file using ffmpeg."""
    wav_file = os.path.splitext(mp3_file)[0] + ".wav"
    ffmpeg.input(mp3_file).output(wav_file, ar='16000').run(overwrite_output=True)
    return wav_file