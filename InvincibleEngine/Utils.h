#pragma once

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

    inline bool IsOverlapping(const Rect& rect1, const Rect& rect2)
    {
        // Check if one rectangle is to the left or above the other
        return !(rect1.Right() <= rect2.position.x ||  // rect1 is to the left of rect2
            rect2.Right() <= rect1.position.x ||  // rect2 is to the left of rect1
            rect1.Bottom() <= rect2.position.y || // rect1 is above rect2
            rect2.Bottom() <= rect1.position.y);  // rect2 is above rect1
    }

    void SaveHighScore(int highScore);
    int LoadHighScore();
}
