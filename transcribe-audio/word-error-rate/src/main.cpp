#include "../include/helper.h"
#include <iostream>
#include <iomanip>

int main()
{
    std::string originalFile, targetFile;
    std::cout << "Enter the path to the original transcription file: ";
    std::cin >> originalFile;
    std::cout << "Enter the path to the target transcription file: ";
    std::cin >> targetFile;

    // Read the transcription files
    std::vector<std::string> originalWords = read_transcription_file(originalFile);
    std::vector<std::string> targetWords = read_transcription_file(targetFile);

    if (originalWords.empty() || targetWords.empty())
    {
        std::cerr << "Error: one or both of the transcription files are empty" << std::endl;
        return 1;
    }

    // Calculate the Word Error Rate (WER)
    float wer =  calculate_wer(originalWords, targetWords);

    std::cout << std::setprecision(4) << "The Word Error Rate (WER) is: " << wer << " %" << std::endl;
}
