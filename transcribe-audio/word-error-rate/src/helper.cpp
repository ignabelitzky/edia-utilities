#include "../include/helper.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

void clean_string(std::string &str) {
    // Convert the string to lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    // Remove punctuation
    str.erase(std::remove_if(str.begin(), str.end(), ::ispunct), str.end());
}

std::vector<std::string> split_into_words(const std::string &str) {
    std::vector<std::string> words;
    std::istringstream iss(str);
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

int levenshtein_distance(const std::vector<std::string> &original, const std::vector<std::string> &target) {
    // Get the lengths of the two strings
    int m = original.size();
    int n = target.size();

    // Create a 2D vector to store the distances
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // Initialize the first row and column
    for (int i = 0; i <= m; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; ++j) {
        dp[0][j] = j;
    }

    // Fill in the rest of the matrix
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int cost = original[i - 1] == target[j - 1] ? 0 : 1;
            dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost});
        }
    }

    // Return the Levenshtein distance
    return dp[m][n];
}

float calculate_wer(const std::vector<std::string> &original, const std::vector<std::string> &target) {
    // Compute the Levenshtein distance
    int distance = levenshtein_distance(original, target);

    // Calculate the Word Error Rate (WER)
    float wer = static_cast<float>(distance) * 100 / original.size();

    return wer;
}

std::vector<std::string> read_transcription_file(const std::string &filename) {
    std::vector<std::string> words;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            clean_string(line);
            std::vector<std::string> line_words = split_into_words(line);
            words.insert(words.end(), line_words.begin(), line_words.end());
        }
        file.close();
    } else {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
    return words;
}