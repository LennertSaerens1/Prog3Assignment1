#include <SDL.h>
#include "iostream"
#include "InputManager.h"
#include "imgui_impl_sdl2.h"
#include <Windows.h>
#include "Xinput.h"


void dae::InputManager::ClearCommands()
{
    // Clear keyboard command maps
	m_keyboardCommandMap.clear();
    m_keyboardUpCommandMap.clear();
    m_keyboardDownCommandMap.clear();

    // Clear controller command maps
    m_controllerCommandMap.clear();
    m_controllerUpCommandMap.clear();
    m_controllerDownCommandMap.clear();

    // Clear controller 2 command maps
    m_controller2CommandMap.clear();
    m_controller2DownCommandMap.clear();
    m_controller2UpCommandMap.clear();
}

bool dae::InputManager::ProcessInput(float elapsedSec)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false; // Handle pressing x
        }
    }

   // Update Keyboard Input
    m_Keyboard.ProcessInput();
	//Update Controller Input
    m_Controller.ProcessInput();
	m_Controller2.ProcessInput();

    ProcessMappedInput<SDL_Scancode>(m_keyboardCommandMap,
        [this](SDL_Scancode key) { return m_Keyboard.IsPressed(key); }, elapsedSec);//Handle isPressed for eacht input on keyboard

    ProcessMappedInput<SDL_Scancode>(m_keyboardDownCommandMap,
        [this](SDL_Scancode key) { return m_Keyboard.IsDownThisFrame(key); }, elapsedSec);//Handle isDownThisFrame for each input on keyboard

    ProcessMappedInput<SDL_Scancode>(m_keyboardUpCommandMap,
        [this](SDL_Scancode key) { return m_Keyboard.IsUpThisFrame(key); }, elapsedSec);//Handle IsUpThisFrame for each input on keyboard


    ProcessMappedInput<int>(m_controllerCommandMap,
        [this](int button) { return m_Controller.IsPressed(button); }, elapsedSec);//Handle isPressed for eacht input on controller

    ProcessMappedInput<int>(m_controllerDownCommandMap,
        [this](int button) { return m_Controller.IsDownThisFrame(button); }, elapsedSec);//Handle isDownThisFrame for each input on controller

    ProcessMappedInput<int>(m_controllerUpCommandMap,
		[this](int button) { return m_Controller.IsUpThisFrame(button); }, elapsedSec);//Handle IsUpThisFrame for each input on controller

    ProcessMappedInput<int>(m_controller2CommandMap,
		[this](int button) { return m_Controller2.IsPressed(button); }, elapsedSec);//Handle isPressed for eacht input on controller 2
    ProcessMappedInput<int>(m_controller2DownCommandMap,
        [this](int button) { return m_Controller2.IsDownThisFrame(button); }, elapsedSec);

    ProcessMappedInput<int>(m_controller2UpCommandMap,
        [this](int button) { return m_Controller2.IsUpThisFrame(button); }, elapsedSec);


    return true;
}

void dae::InputManager::BindKeyboardCommand(SDL_Scancode key, std::shared_ptr<Command> command) {
    m_keyboardCommandMap[key].push_back(command);
}

void dae::InputManager::BindControllerCommand(int button, std::shared_ptr<Command> command) {
    m_controllerCommandMap[button].push_back(command);
}

void dae::InputManager::BindKeyboardUpCommand(SDL_Scancode key, std::shared_ptr<Command> command)
{
    m_keyboardUpCommandMap[key].push_back(command);
}

void dae::InputManager::BindControllerUpCommand(int button, std::shared_ptr<Command> command)
{
    m_controllerUpCommandMap[button].push_back(command);
}

void dae::InputManager::BindKeyboardDownCommand(SDL_Scancode key, std::shared_ptr<Command> command)
{
    m_keyboardDownCommandMap[key].push_back(command);
}

void dae::InputManager::BindControllerDownCommand(int button, std::shared_ptr<Command> command)
{
    m_controllerDownCommandMap[button].push_back(command);
}

void dae::InputManager::BindController2Command(int button, std::shared_ptr<Command> command)
{
    m_controller2CommandMap[button].push_back(command);
}
void dae::InputManager::BindController2DownCommand(int button, std::shared_ptr<Command> command)
{
    m_controller2DownCommandMap[button].push_back(command);
}

void dae::InputManager::BindController2UpCommand(int button, std::shared_ptr<Command> command)
{
    m_controller2UpCommandMap[button].push_back(command);
}



//void dae::InputManager::HandleKeyboardInput(int key, float elapsedSec) {
//    // Check if the key exists in the keyboard command map
//    if (m_keyboardCommandMap.find(key) != m_keyboardCommandMap.end()) {
//        auto& commands = m_keyboardCommandMap[key];
//        // Loop through the list of commands associated with the key and execute them
//        for (auto& command : commands) {
//            command->Execute(elapsedSec);
//        }
//    }
//}//Iterate over commands not keys
//
//void dae::InputManager::HandleControllerInput(int button, float elapsedSec) {
//    // Check if the button exists in the controller command map
//    if (m_controllerCommandMap.find(button) != m_controllerCommandMap.end()) {
//        auto& commands = m_controllerCommandMap[button];
//        // Loop through the list of commands associated with the button and execute them
//        for (auto& command : commands) {
//            command->Execute(elapsedSec);
//        }
//    }
//}

// Unbind a command from a specific keyboard input
void dae::InputManager::UnbindKeyboardCommand(SDL_Scancode input, std::shared_ptr<Command> command) {
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

void dae::InputManager::UnbindKeyboardUpCommand(SDL_Scancode input, std::shared_ptr<Command> command)
{
    if (m_keyboardUpCommandMap.find(input) != m_keyboardUpCommandMap.end()) {
        auto& commands = m_keyboardUpCommandMap[input];
        commands.erase(std::remove(commands.begin(), commands.end(), command), commands.end());
        if (commands.empty()) {
            m_keyboardUpCommandMap.erase(input);  // Clean up empty entries
        }
    }
}

void dae::InputManager::UnbindControllerUpCommand(int input, std::shared_ptr<Command> command)
{
    if (m_controllerUpCommandMap.find(input) != m_controllerUpCommandMap.end()) {
        auto& commands = m_controllerUpCommandMap[input];
        commands.erase(std::remove(commands.begin(), commands.end(), command), commands.end());
        if (commands.empty()) {
            m_controllerUpCommandMap.erase(input);  // Clean up empty entries
        }
    }
}

void dae::InputManager::UnbindKeyboardDownCommand(SDL_Scancode input, std::shared_ptr<Command> command)
{
    if (m_keyboardDownCommandMap.find(input) != m_keyboardDownCommandMap.end()) {
        auto& commands = m_keyboardDownCommandMap[input];
        commands.erase(std::remove(commands.begin(), commands.end(), command), commands.end());
        if (commands.empty()) {
            m_keyboardDownCommandMap.erase(input);  // Clean up empty entries
        }
    }
}

void dae::InputManager::UnbindControllerDownCommand(int input, std::shared_ptr<Command> command)
{
    if (m_controllerDownCommandMap.find(input) != m_controllerDownCommandMap.end()) {
        auto& commands = m_controllerDownCommandMap[input];
        commands.erase(std::remove(commands.begin(), commands.end(), command), commands.end());
        if (commands.empty()) {
            m_controllerDownCommandMap.erase(input);  // Clean up empty entries
        }
    }
}

void dae::InputManager::UnbindController2Command(int input, std::shared_ptr<Command> command)
{
    if (m_controller2CommandMap.find(input) != m_controller2CommandMap.end()) {
        auto& commands = m_controller2CommandMap[input];
        commands.erase(std::remove(commands.begin(), commands.end(), command), commands.end());
        if (commands.empty()) {
            m_controller2CommandMap.erase(input);  // Clean up empty entries
        }
    }
}

void dae::InputManager::UnbindController2DownCommand(int input, std::shared_ptr<Command> command)
{
    if (m_controller2DownCommandMap.find(input) != m_controller2DownCommandMap.end()) {
        auto& commands = m_controller2DownCommandMap[input];
        commands.erase(std::remove(commands.begin(), commands.end(), command), commands.end());
        if (commands.empty()) {
            m_controller2DownCommandMap.erase(input);
        }
    }
}

void dae::InputManager::UnbindController2UpCommand(int input, std::shared_ptr<Command> command)
{
    if (m_controller2UpCommandMap.find(input) != m_controller2UpCommandMap.end()) {
        auto& commands = m_controller2UpCommandMap[input];
        commands.erase(std::remove(commands.begin(), commands.end(), command), commands.end());
        if (commands.empty()) {
            m_controller2UpCommandMap.erase(input);
        }
    }
}
