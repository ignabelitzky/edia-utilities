import os
import subprocess


def convert_mp3_to_wav(mp3_file):
    """Convert an MP3 file to a WAV file using ffmpeg.
    
    Args:
        mp3_file (str): The input MP3 file path.

    Returns:
        str: The output WAV file path if successful, None otherwise.
    """
    try:
        # Define the output WAV file
        wav_file = os.path.splitext(mp3_file)[0] + ".wav"

        # Construct the ffmpeg command to convert MP3 to WAV
        command = [
            'ffmpeg',
            '-i', mp3_file,
            '-ac', '1',                 # Set the number of audio channels to 1 (mono)
            '-ar', '16000',             # Set the audio sampling rate to 16 kHz
            '-acodec', 'pcm_s16le',     # Set the audio codec to 16-bit PCM
            wav_file
        ]

        # Run the ffmpeg command
        subprocess.run(command, check=True)
        print(f"MP3 successfully converted to WAV: {wav_file}")
        return wav_file
    except subprocess.CalledProcessError as e:
        print(f"Error converting MP3 to WAV: {e}")
        return None
