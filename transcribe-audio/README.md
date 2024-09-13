# MP3 to Text Transcription Tool

This Python tool allows you to select an MP3 file, convert it to WAV format, and transcribe it using OpenAI's Whisper model. The transcription is saved to a location of your choice in a text file.

## Features

- Simple GUI for selecting MP3 files and specifying save locations.
- Converts MP3 to WAV format to improve transcription accuracy.
- Allows selection of Whisper models and transcription language (English or Spanish)
- Saves transcriptions with timestamps to a `.txt` file.
- Modular design with clear separation of concerns.

## Requirements

### Python Libraries

Ensure the following Python libraries are installed:
1. **Tkinter** (for GUI): Tkinter is usually pre-installed with Python.
2. **Whisper** (OpenAI's transcription model):
    ```bash
    pip install whisper
    ```
3. **Torch** (required for Whisper):
    ```bash
    pip install torch
    ```
4. **FFmpeg** (for converting MP3 to WAV):
    ```bash
    pip install ffmpeg-python
    ```

### FFmpeg

You also need to install `ffmpeg` on your system:
- **Ubuntu**:
    ```bash
    sudo apt-get install ffmpeg
    ```
- **macOS** (using Homebrew):
    ```bash
    brew install ffmpeg
    ```
- **Windows**:
    1. Download FFmpeg from the official site: [FFmpeg Downloads](https://ffmpeg.org/download.html).
    2. Add FFmpeg to your system PATH (see instructions on the FFmpeg website).

## Installation

1. Clone this repository (or download the `transcribe_audio.py` script).
    ```bash
    git clone https://www.github.com/ignabelitzky/edia-utilities
    ```
2. Install the required Python packages.
    ```bash
    pip install -r requirements.txt
    ```
3. Ensure that `ffmpeg` is installed and available in your system's PATH.

## Project Structure

The program is modularized into the following components:
- **file_dialogs.py**: Handles file selection and save location dialogs.
- **conversion.py**: Converts MP3 files to WAV format using FFmpeg.
- **options.py**: Provides GUI for selection Whisper model and transcription language.
- **transcription.py**: Manages the transcription process and timestamp formatting.
- **main.py**: The entry point of the program that integrates all the modules.

## Usage

1. Run the main script to start the program:
    ```bash
    python main.py
    ```
2. The GUI will guide you through the following steps:
    - **Select MP3 File**: Use the file dialog to choose the MP3 file for transcription.
    - **Choose Whisper Model**: Select the Whisper model (e.g., tiny, base, small, medium, large) from a dropdown menu.
    - **Choose Language**: Select the transcription language (English or Spanish).
    - **Save Transcription**: Choose a location to ave the `.txt` file containing the transcription.
3. After the transcription process, the progam will display a success message, and the transcription will be saved at the specified location.
4. The WAV file created during the transcription process will be automatically deleted after the transcription is complete.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE). You can find the full text of the license [here](LICENSE)
