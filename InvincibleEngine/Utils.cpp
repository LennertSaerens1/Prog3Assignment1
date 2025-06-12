#include "Utils.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

namespace utils
{
    constexpr int MAX_HIGH_SCORES = 10;
    const std::string HIGH_SCORE_FILE = "HighScore.txt";

    std::vector<int> LoadAllHighScores()
    {
        std::vector<int> highScores;
        std::ifstream inFile(HIGH_SCORE_FILE);

        if (inFile.is_open())
        {
            int score;
            while (inFile >> score)
            {
                highScores.push_back(score);
            }
            inFile.close();
        }
        else
        {
            std::cerr << "Error: Unable to open " << HIGH_SCORE_FILE << " for reading. Creating empty high scores list." << std::endl;
        }

        std::sort(highScores.begin(), highScores.end(), std::greater<int>());

        if (highScores.size() > MAX_HIGH_SCORES)
        {
            highScores.resize(MAX_HIGH_SCORES);
        }

        return highScores;
    }

    // Save high scores to file
    void SaveHighScores(const std::vector<int>& highScores)
    {
        std::ofstream outFile(HIGH_SCORE_FILE);
        if (outFile.is_open())
        {
            for (int score : highScores)
            {
                outFile << score << std::endl;
            }
            outFile.close();
        }
        else
        {
            std::cerr << "Error: Unable to open " << HIGH_SCORE_FILE << " for writing." << std::endl;
        }
    }

    void SaveHighScore(int highScore)
    {
        std::vector<int> highScores = LoadAllHighScores();

        // If we already have MAX_HIGH_SCORES and the new score is lower than all of them, we don't need to add it
        if (highScores.size() >= MAX_HIGH_SCORES && highScore <= highScores.back())
        {
            return;
        }

        // Insert the new score
        highScores.push_back(highScore);

        // Sort in descending order
        std::sort(highScores.begin(), highScores.end(), std::greater<int>());

        // Truncate to MAX_HIGH_SCORES if needed
        if (highScores.size() > MAX_HIGH_SCORES)
        {
            highScores.resize(MAX_HIGH_SCORES);
        }

        SaveHighScores(highScores);
    }

    int LoadHighScore()
    {
        std::vector<int> highScores = LoadAllHighScores();

        if (!highScores.empty())
        {
            return highScores[0]; // Return the highest score
        }

        return 0; // Default if no scores exist
    }

    std::vector<int> GetTop10Scores()
    {
        return LoadAllHighScores();
    }
}