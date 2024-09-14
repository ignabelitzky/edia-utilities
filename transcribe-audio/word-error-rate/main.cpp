#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>

// Function to split a string into words
std::vector<std::string> split_into_words(const std::string &str)
{
    std::vector<std::string> words;
    std::stringstream ss(str);
    std::string word;
    while (ss >> word)
    {
        words.push_back(word);
    }
    return words;
}

// Function to compute the Levenshtein distance
int levenshtein_distance(const std::vector<std::string> &original, const std::vector<std::string> &target)
{
    int m = original.size();
    int n = target.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    // Initialize dp table
    for (int i = 0; i <= m; ++i)
    {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; ++j)
    {
        dp[0][j] = j;
    }

    // Fill dp table
    for (int i = 1; i <= m; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            if (original[i - 1] == target[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else
            {
                dp[i][j] = std::min({dp[i - 1][j - 1], dp[i][j - 1], dp[i - 1][j]}) + 1;
            }
        }
    }
    return dp[m][n];
}

// Function to calculate the Word Error Rate (WER)
float calculate_wer(const std::vector<std::string> &original, const std::vector<std::string> &target)
{
    int distance = levenshtein_distance(original, target);
    return static_cast<float> (distance * 100) / original.size();
}

// Function to read a transcription file and return the words
std::vector<std::string> read_transcription_file(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return {};
    }
    
    std::string line = "", content = "";
    while (std::getline(file, line))
    {
        content += line + " ";
    }
    file.close();
    return split_into_words(content);
}

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