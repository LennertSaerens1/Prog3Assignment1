#include "KeyBoardInput.h"

namespace dae
{
    KeyBoardInput::KeyBoardInput()
    {
        m_keyboardState = SDL_GetKeyboardState(nullptr); // Get the current state of all keys
        std::memset(m_previousState, 0, sizeof(m_previousState)); // Initialize previous state
    }

    void KeyBoardInput::ProcessInput()
    {
        // Store the current state as the previous state
        std::memcpy(m_previousState, m_keyboardState, sizeof(m_previousState));

        // Update the current state of all keys
        SDL_PumpEvents();
        m_keyboardState = SDL_GetKeyboardState(nullptr);
    }

    bool KeyBoardInput::IsDownThisFrame(SDL_Scancode key) const
    {
        // Check if the key was not pressed before, but is pressed now
        return (m_keyboardState[key] && !m_previousState[key]);
    }

    bool KeyBoardInput::IsUpThisFrame(SDL_Scancode key) const
    {
        // Check if the key was pressed before, but is not pressed now
        return (!m_keyboardState[key] && m_previousState[key]);
    }

    bool KeyBoardInput::IsPressed(SDL_Scancode key) const
    {
        // Check if the key is currently pressed
        return m_keyboardState[key];
    }
}
