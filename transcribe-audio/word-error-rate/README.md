# Word Error Rate (WER) Calculator

This C++ program calculates the Word Error Rate (WER) between two transcription files. WER is a common metric used to evaluate the accuracy of speech recognition systems by comparing the output transcription to a reference transcription.

## Overview

The program performs the following tasks:
1. Reads two transcription files (original and target)
2. Computes the Levenshtein distance between the words in both transcriptions.
3. Calculates the Word Error Rate (WER) based on the Levenshtein distance
4. Outputs the WER to the console.

## Functions

- `split_into_words(const std::string &str)`: Splits a string into a vector of words.
- `levenshtein_distance(const std::vector<std::string> &original, const std::vector<std::string> &target)`: Computes the Levenshtein distance between two vectors of words.
- `calculate_wer(const std::vector<std::string> &original, const std::vector<std::string> &target)`: Calculates the Word Error Rate (WER) based on the Levenshtein distance.
- `read_transcription_file(const std::string &filename)`: Reads a transcription file and returns a vector of words.

## Compilation

To compile the program, use a C++ compiler like `g++`. Run the following command in the terminal:

```bash
g++ -o wer_calculator main.cpp
```

## Usage

After compilation, you can run the program with the following command:

```bash
./wer_calculator
```

The program will prompt you to enter the paths to the original and target transcription files. Enter the full paths to these files, and the program will calculate and display the Word Error Rate.

## Notes

- Ensure that both transcription files are plain text files with one or more words per line.
- The program assumes that words in the transcription files are separated by spaces.
- The WER is expressed as a percentage, where a lower percentage indicates a better match between the original and target transcriptions.

## License

This program is licensed under the [GNU General Public License v3.0](../LICENSE). You can find the full text of the license here [LICENSE](../LICENSE).