#pragma once
#include <SDL.h>
#include <iostream>

namespace dae
{
    class KeyBoardInput
    {
    public:
        KeyBoardInput();
        void ProcessInput();
        bool IsDownThisFrame(SDL_Scancode key) const;
        bool IsUpThisFrame(SDL_Scancode key) const;
        bool IsPressed(SDL_Scancode key) const;

    private:
        const Uint8* m_keyboardState; // Current state of all keys
        Uint8 m_previousState[SDL_NUM_SCANCODES]; // Previous state of all keys
        Uint8 m_currentState[SDL_NUM_SCANCODES]; // Copy of current state for safe comparison
    };
}
