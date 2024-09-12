#!/bin/bash

# Function to download the best available video with audio
download_video() {
    yt-dlp --progress -f bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4] "$1" -o "$2"
    check_status
}

# Function to download audio only in MP3 format
download_audio() {
    yt-dlp --progress -x --audio-format mp3 "$1" -o "$2"
    check_status
}

# Function to download both video and audio separately
download_both() {
    download_video "$1" "$2"
    download_audio "$1" "$2"
}

# Function to check if yt-dlp is installed
check_dependencies() {
    echo "Checking if yt-dlp is installed..."
    if ! command -v yt-dlp &>/dev/null; then
        echo "yt-dlp is not installed. Please install it before running this script."
        exit 1
    fi
    echo "yt-dlp is installed. Proceeding with the script..."
}

# function to check the status of the last command
check_status() {
    if [ $? -ne 0 ]; then
        echo "An error occurred. Please check the output for more details."
        exit 1
    fi
}

# Function to display the menu
show_menu() {
    echo
    echo "=========================="
    echo "    YouTube Downloader"
    echo "=========================="
    echo
    echo "Do you want to download the video or just the audio?"
    echo "1. Video"
    echo "2. Audio (mp3)"
    echo "3. Both (video and audio)"
    echo "4. Exit"
    read -p "Choose an option: " choice
}

# Start the script
check_dependencies

choice=0

while true; do
    show_menu

    case $choice in
    1|2|3)
        read -p "Enter the YouTube URL: " url
        if [[ ! $url =~ ^https?://(www\.)?youtube\.com|youtube\.be/.+$ ]]; then
            echo "Invalid YouTube URL. Please enter a valid URL."
            continue
        fi
        read -p "Enter the output directory (empty for ~/Downloads): " output_dir
        if [ -z "$output_dir" ]; then
            output_dir="$HOME/Downloads"
        elif [ ! -d "$output_dir" ]; then
            echo "Directory does not exist. Creating directory..."
            mkdir -p "$output_dir"
        fi
        read -p "Enter the output filename (empty for default): " output_filename
        if [ -z "$output_filename" ]; then
            output_filename="%(title)s.%(ext)s"
        fi
        ;;
    4)
        echo "Exiting..."
        exit 0
        ;;
    *)
        echo "Invalid choice. Please choose a valid option."
        continue
        ;;
    esac

    case $choice in
    1)
        download_video "$url" "$output_dir/$output_filename"
        echo "Download complete. File saved to: $output_dir/$output_filename"
        ;;
    2)
        download_audio "$url" "$output_dir/$output_filename"
        echo "Download complete. File saved to: $output_dir/$output_filename"
        ;;
    3)
        download_both "$url" "$output_dir/$output_filename"
        echo "Download complete. Files saved to: $output_dir"
        ;;
    esac
done

