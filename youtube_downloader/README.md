# YouTube Downloader

This is a simple Bash script for downloading YouTube videos and audio files using yt-dlp. The script provides an interactive menu to choose whether you want to download the video, the audio in MP3 format, or both.

## Features

- **Video Download:** Downloads the best available video format (MP4) with audio.
- **Audio Download:** Extracts and downloads audio in MP3 format.
- **Dual Download:** Downloads both video and audio separately.
- **Progress Display:** Shows download progress for both video and audio.
- **URL Validation:** Ensures the entered URL is valid before attempting to download.
- **Dependency Check:** Verifies that `yt-dlp` is installed before running.

## Prerequisites

- **yt-dlp:** This script requires `yt-dlp` to be installed on your system.

You can install it using the following commands:
```bash
sudo curl -L https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp -o /usr/local/bin/yt-dlp
sudo chmod a+rx /usr/local/bin/yt-dlp
```

## Usage

1. **Run the Script:**
```bash
./youtube_downloader.sh
```
2. **Choose an Option:**
- 1. **Video:** Downloads the best available video with audio.
- 2. **Audio (mp3):** Downloads only the aduio in MP3 format.
- 3. **Both (video and audio):** Downloads both video and audio separately.
- 4. **Exit:** Exits the script.

3. **Enter the YouTube Video URL:**
- The script will prompt you to enter the URL of the YouTube video you want to download.

4. **Download Location:**
- The downloaded files will be saved in the `~/Downloads` directory, named after the title of the YouTube video.

## Notes
- Ensure that you have a stable internet connection while using the script.
- Make sure the `yt-dlp` tool is kept up to date for the best performance and compatibility with YouTube.

## License
This project is licensed under the [GNU General Public License v3.0](LICENSE). You can find the full text of the license here [LICENSE](LICENSE).