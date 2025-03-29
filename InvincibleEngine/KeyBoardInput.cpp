#include "KeyBoardInput.h"

namespace dae
{
    KeyBoardInput::KeyBoardInput()
    {
        m_keyboardState = SDL_GetKeyboardState(nullptr);
        // Initialize both state arrays
        std::memset(m_previousState, 0, sizeof(m_previousState));
        std::memset(m_currentState, 0, sizeof(m_currentState));
    }

    void KeyBoardInput::ProcessInput()
    {
        // Copy current state to previous state
        std::memcpy(m_previousState, m_currentState, sizeof(m_previousState));

        // Update keyboard state pointer and copy to our own buffer
        SDL_PumpEvents();
        m_keyboardState = SDL_GetKeyboardState(nullptr);
        std::memcpy(m_currentState, m_keyboardState, sizeof(m_currentState));
    }

    bool KeyBoardInput::IsDownThisFrame(SDL_Scancode key) const
    {
        // Key is pressed now but wasn't pressed last frame
        return (m_currentState[key] && !m_previousState[key]);
    }

    bool KeyBoardInput::IsUpThisFrame(SDL_Scancode key) const
    {
        // Key is not pressed now but was pressed last frame
        return (!m_currentState[key] && m_previousState[key]);
    }

    bool KeyBoardInput::IsPressed(SDL_Scancode key) const
    {
        // Check if the key is currently pressed
        return m_keyboardState[key];
    }
}
