# MP3 to Text Transcription Tool
This Python tool allows you to select an MP3 file from any directory, convert it to WAV format, and transcribe it using OpenAI's Whisper model. The transcription is saved to a location of your choice in a text file.

## Features
- Simple GUI for file selection and save location.
- Converts MP3 to WAV for better transcription accuracy.
- Uses OpenAI Whisper model to transcribe Spanish audio.
- Saves the transcription to a `.txt` file.

## Requirements
Make sure the following dependencies are installed before running the script:
1. **Python Libraries**
    - **Tkinter** (for GUI file dialog): Tkinter is usually pre-installed with Python.
    - **Whisper** (OpenAI's transcription model):
        ```bash
        pip install whisper
        ```
    - **Torch** (required for Whisper):
        ```bash
        pip install torch
        ```
    - **FFmpeg** (for converting MP3 to WAV):
        ```bash
        pip install ffmpeg-python
        ```
2. **FFmpeg**
You also need the `ffmpeg` tool to convert MP3 files to WAV format.
On Ubuntu:
```bash
sudo apt-get install ffmpeg
```

On macOS using Homebrew:
```bash
brew install ffmpeg
```

On Windows:
1. Download FFmpeg from the official site: [FFmpeg Downloads](https://ffmpeg.org/download.html).
2. Add FFmpeg to your system PATH.

## Installation
1. Clone this repository (or download the `transcribe_audio.py` script).
    ```bash
    git clone https://www.github.com/ignabelitzky/edia-utilities
    ```
2. Install the required Python packages.
    ```bash
    pip install -r requirements.txt
    ```
3. Ensure that `ffmpeg is installed and available in your system's PATH.

## Usage
1. Run the Python script:
    ```bash
    python transcribe_audio.py
    ```
2. A file dialog will appear to select an MP3 file from your directory.
3. After selecting the file, the script will convert the MP3 file to WAV format.
4. Another dialog will appear asking where you'd like to save the transcription file. Choose the desired save location.
5. Once the transcription is complete, you will get a success message, and the transcription will be saved in the chosen location.

## License
This project is licensed under the [GNU General Public License v3.0](LICENSE). You can find the full text of the license here [LICENSE](LICENSE)
