#pragma once
#include <vector>


namespace utils
{
    struct Vector2f
    {
        float x{};
        float y{};
    };

    struct Rect
    {
        Vector2f position; // Top-left corner
        float width{};
        float height{};

        // Helper function to get the right and bottom edges
        float Right() const { return position.x + width; }
        float Bottom() const { return position.y + height; }
    };

    struct HighScoreEntry {
        int score;
        char name[3];

        HighScoreEntry(int s = 0, const char* n = "AAA") : score(s) {
            name[0] = n[0];
            name[1] = n[1];
            name[2] = n[2];
        }
    };

    inline bool IsOverlapping(const Rect& rect1, const Rect& rect2)
    {
        // Check if one rectangle is to the left or above the other
        return !(rect1.Right() <= rect2.position.x ||  // rect1 is to the left of rect2
            rect2.Right() <= rect1.position.x ||  // rect2 is to the left of rect1
            rect1.Bottom() <= rect2.position.y || // rect1 is above rect2
            rect2.Bottom() <= rect1.position.y);  // rect2 is above rect1
    }

    int LoadHighScore();
    std::vector<HighScoreEntry> LoadAllHighScoreEntries();
    void SaveHighScoresWithNames(const std::vector<HighScoreEntry>& highScores);
    void SaveHighScore(int highScore, const char name[3] = "AAA");
}
