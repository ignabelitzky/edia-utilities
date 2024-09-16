#ifndef HELPER_H_
#define HELPER_H_

#include <string>
#include <vector>

// Function to clean a string by converting it to lowercase and removing punctuation
void clean_string(std::string &str);

// Function to split a string into words
std::vector<std::string> split_into_words(const std::string &str);

// Function to compute the Levenshtein distance
int levenshtein_distance(const std::vector<std::string> &original, const std::vector<std::string> &target);

// Function to calculate the Word Error Rate (WER)
float calculate_wer(const std::vector<std::string> &original, const std::vector<std::string> &target);

// Function to read a transcription file and return the words
std::vector<std::string> read_transcription_file(const std::string &filename);

#endif