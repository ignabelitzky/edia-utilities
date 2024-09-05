# Text File Formatter

## Overview
The `text_file_formatter.py` script is a simple Python application that formats text files by inserting new lines after eery 13 words. It features a graphical user interface (GUI) created with `tkinter` for easy file selection and processing.

## Features
- **File Selection**: Choose input and output files using file dialog boxes.
- **Formatting**: Insert new lines after every 13 words in the input text file.
- **Save Output**: Save the formatted text to a new file.

## Requirements
- Python 3.x
- `tkinter` (comes pre-installed with Python)

## Installation
1. **Clone or Download the Repository**:
    ``` bash
    git clone https://github.com/ignabelitzky/edia-utilities
    ```
    or download the script directly from the repository.
2. **Navigate to the Script Directory**:
    ```bash
    cd edia-utilities/text_file_formatter
    ```
3. **Run the Script**:
    ```bash
    python text_file_formatter.py
    ```

## Usage
1. **Open the GUI**: Run the script to oen the graphical user interface.
2. **Select Files**:
    - Click "Browse" next to "Input File" to choose the text file you want to format.
    - Click "Browse" next to "Output File" to choose where to save the formatted text.
3. **Process the File**: Click "Process File" to format the text file. A message will appear indicating the success or failure of the operation.

## License
This project is licensed under the [GNU General Public License v3.0](LICENSE). You can find the full text of the license here [LICENSE](LICENSE)