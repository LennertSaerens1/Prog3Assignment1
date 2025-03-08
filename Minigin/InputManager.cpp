#include <SDL.h>
#include "iostream"
#include "InputManager.h"
#include "imgui_impl_sdl2.h"
#include <Windows.h>
#include "Xinput.h"

bool dae::InputManager::ProcessInput(float elapsedSec)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false; // Handle pressing x
        }
    }

   // Process Keyboard Input
    m_Keyboard.ProcessInput();
    if (m_Keyboard.IsPressed(SDL_SCANCODE_W)) {
        HandleKeyboardInput(SDL_SCANCODE_W, elapsedSec);  // Handle Keyboard Input
    }
    if (m_Keyboard.IsPressed(SDL_SCANCODE_A)) {
        HandleKeyboardInput(SDL_SCANCODE_A, elapsedSec);  // Handle Keyboard Input
    }
    if (m_Keyboard.IsPressed(SDL_SCANCODE_S)) {
        HandleKeyboardInput(SDL_SCANCODE_S, elapsedSec);  // Handle Keyboard Input
    }
    if (m_Keyboard.IsPressed(SDL_SCANCODE_D)) {
        HandleKeyboardInput(SDL_SCANCODE_D, elapsedSec);  // Handle Keyboard Input
    }

    // Process Controller Input
    m_Controller.ProcessInput();
    if (m_Controller.IsPressed(XINPUT_GAMEPAD_DPAD_UP)) {
        HandleControllerInput(XINPUT_GAMEPAD_DPAD_UP, elapsedSec);  // Handle Controller Input
    }
    if (m_Controller.IsPressed(XINPUT_GAMEPAD_DPAD_DOWN)) {
        HandleControllerInput(XINPUT_GAMEPAD_DPAD_DOWN, elapsedSec);  // Handle Controller Input
    }
    if (m_Controller.IsPressed(XINPUT_GAMEPAD_DPAD_LEFT)) {
        HandleControllerInput(XINPUT_GAMEPAD_DPAD_LEFT, elapsedSec);  // Handle Controller Input
    }
    if (m_Controller.IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) {
        HandleControllerInput(XINPUT_GAMEPAD_DPAD_RIGHT, elapsedSec);  // Handle Controller Input
    }

    return true;
}

void dae::InputManager::BindKeyboardCommand(int key, std::shared_ptr<Command> command) {
    m_keyboardCommandMap[key].push_back(command);
}

void dae::InputManager::BindControllerCommand(int button, std::shared_ptr<Command> command) {
    m_controllerCommandMap[button].push_back(command);
}

void dae::InputManager::HandleKeyboardInput(int key, float elapsedSec) {
    // Check if the key exists in the keyboard command map
    if (m_keyboardCommandMap.find(key) != m_keyboardCommandMap.end()) {
        auto& commands = m_keyboardCommandMap[key];
        // Loop through the list of commands associated with the key and execute them
        for (auto& command : commands) {
            command->Execute(elapsedSec);
        }
    }
}

void dae::InputManager::HandleControllerInput(int button, float elapsedSec) {
    // Check if the button exists in the controller command map
    if (m_controllerCommandMap.find(button) != m_controllerCommandMap.end()) {
        auto& commands = m_controllerCommandMap[button];
        // Loop through the list of commands associated with the button and execute them
        for (auto& command : commands) {
            command->Execute(elapsedSec);
        }
    }
}

// Unbind a command from a specific keyboard input
void dae::InputManager::UnbindKeyboardCommand(int input, std::shared_ptr<Command> command) {
    if (m_keyboardCommandMap.find(input) != m_keyboardCommandMap.end()) {
        auto& commands = m_keyboardCommandMap[input];
        commands.erase(std::remove(commands.begin(), commands.end(), command), commands.end());
        if (commands.empty()) {
            m_keyboardCommandMap.erase(input);  // Clean up empty entries
        }
    }
}

// Unbind a command from a specific controller input
void dae::InputManager::UnbindControllerCommand(int input, std::shared_ptr<Command> command) {
    if (m_controllerCommandMap.find(input) != m_controllerCommandMap.end()) {
        auto& commands = m_controllerCommandMap[input];
        commands.erase(std::remove(commands.begin(), commands.end(), command), commands.end());
        if (commands.empty()) {
            m_controllerCommandMap.erase(input);  // Clean up empty entries
        }
    }
}

