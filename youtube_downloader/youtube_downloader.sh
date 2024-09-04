#!/bin/bash

echo "=========================="
echo "    YouTube Downloader"
echo "=========================="
echo

# Function to download the best available video with audio
download_video() {
    yt-dlp --progress -f bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4] "$1" -o "~/Downloads/%(title)s.%(ext)s"
}

# Function to download audio only in MP3 format
download_audio() {
    yt-dlp --progress -x --audio-format mp3 "$1" -o "~/Downloads/%(title)s.%(ext)s"
}

# Function to download both video and audio separately
download_both() {
    download_video "$1"
    download_audio "$1"
}

# Function to check if yt-dlp is installed
check_dependencies() {
    echo "Checking if yt-dlp is installed..."
    if ! command -v yt-dlp &> /dev/null; then
        echo "yt-dlp is not installed. Please install it before running this script."
        return 1
    else
        echo "yt-dlp is installed. Proceeding with the script..."
        return 0
    fi
}

# Start the script
if ! check_dependencies; then
    exit 1
fi

choice=0

while [ "$choice" -ne 4 ]; do
    echo
    echo "Do you want to download the video or just the audio?"
    echo "1. Video"
    echo "2. Audio (mp3)"
    echo "3. Both (video and audio)"
    echo "4. Exit"
    read -p "Choose an option: " choice

    if ! [[ $choice =~ ^[1-4]$ ]]; then
        echo "Invalid choice. Please choose a valid option."
        continue
    fi

    if [ "$choice" -ne 4 ]; then
        read -p "Enter the YouTube video URL: " video_url
        if [[ ! "$video_url" =~ ^https?://(www\.)?(youtube\.com|youtu\.be)/.+$ ]]; then
            echo "Invalid YouTube URL. Please enter a valid URL."
            continue
        fi
    fi

    case $choice in
    1)
        download_video "$video_url"
        ;;
    2)
        download_audio "$video_url"
        ;;
    3)
        download_both "$video_url"
        ;;
    4)
        echo "Exiting..."
        exit 0
        ;;
    esac

    echo "Download complete!"
done
