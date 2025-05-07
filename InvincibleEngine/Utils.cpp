#include "Utils.h"
#include <fstream>
#include <iostream>

namespace utils
{
    void SaveHighScore(int highScore)
    {
		int currentHighScore = LoadHighScore();

        if (highScore < currentHighScore) return;

        std::ofstream outFile("HighScore.txt");
        if (outFile.is_open())
        {
            outFile << highScore;
            outFile.close();
        }
        else
        {
            std::cerr << "Error: Unable to open HighScore.txt for writing." << std::endl;
        }
    }

    int LoadHighScore()
    {
        std::ifstream inFile("HighScore.txt");
        int highScore = 0;

        if (inFile.is_open())
        {
            inFile >> highScore;
            inFile.close();
        }
        else
        {
            std::cerr << "Error: Unable to open HighScore.txt for reading. Returning 0 as default." << std::endl;
        }

        return highScore;
    }
}
