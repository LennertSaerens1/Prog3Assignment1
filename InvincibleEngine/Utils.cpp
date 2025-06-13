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
            std::string nameDummy; // To read and discard names
            while (inFile >> score)
            {
                highScores.push_back(score);
                inFile >> nameDummy; // Read and discard the name
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
                outFile << score << " AAA" << std::endl; // Default name for backward compatibility
            }
            outFile.close();
        }
        else
        {
            std::cerr << "Error: Unable to open " << HIGH_SCORE_FILE << " for writing." << std::endl;
        }
    }

    void SaveHighScoresWithNames(const std::vector<HighScoreEntry>& highScores)
    {
        std::ofstream outFile(HIGH_SCORE_FILE);
        if (outFile.is_open())
        {
            for (const auto& entry : highScores)
            {
                outFile << entry.score << " " << entry.name[0] << entry.name[1] << entry.name[2] << std::endl;
            }
            outFile.close();
        }
        else
        {
            std::cerr << "Error: Unable to open " << HIGH_SCORE_FILE << " for writing." << std::endl;
        }
    }

    std::vector<HighScoreEntry> LoadAllHighScoreEntries()
    {
        std::vector<HighScoreEntry> entries;
        std::ifstream inFile(HIGH_SCORE_FILE);

        if (inFile.is_open())
        {
            int score;
            std::string nameStr;
            while (inFile >> score >> nameStr)
            {
                char name[3] = { 'A', 'A', 'A' };
                if (nameStr.length() >= 3) {
                    name[0] = nameStr[0];
                    name[1] = nameStr[1];
                    name[2] = nameStr[2];
                }
                entries.push_back(HighScoreEntry(score, name));
            }
            inFile.close();
        }
        else
        {
            std::cerr << "Error: Unable to open " << HIGH_SCORE_FILE << " for reading." << std::endl;
        }

        std::sort(entries.begin(), entries.end(),
            [](const HighScoreEntry& a, const HighScoreEntry& b) { return a.score > b.score; });

        if (entries.size() > MAX_HIGH_SCORES)
        {
            entries.resize(MAX_HIGH_SCORES);
        }

        return entries;
    }

    void SaveHighScore(int highScore, const char name[3])
    {
        std::vector<HighScoreEntry> entries = LoadAllHighScoreEntries();

        // Check if we need to add the new score
        if (entries.size() >= MAX_HIGH_SCORES && highScore <= entries.back().score)
        {
            return;
        }

        // Add the new entry
        entries.push_back(HighScoreEntry(highScore, name));

        // Sort and resize
        std::sort(entries.begin(), entries.end(),
            [](const HighScoreEntry& a, const HighScoreEntry& b) { return a.score > b.score; });

        if (entries.size() > MAX_HIGH_SCORES)
        {
            entries.resize(MAX_HIGH_SCORES);
        }

        SaveHighScoresWithNames(entries);
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