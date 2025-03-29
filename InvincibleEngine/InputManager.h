#pragma once
#include "Singleton.h"
#include "Controller.h"
#include "KeyBoardInput.h"
#include <unordered_map>
#include <functional>
#include "Commands.h"
#include <Windows.h>

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);

		// Bind keys/buttons to commands
		void BindKeyboardCommand(SDL_Scancode key, std::shared_ptr<Command> command);
		void BindControllerCommand(int button, std::shared_ptr<Command> command);
		void BindKeyboardUpCommand(SDL_Scancode key, std::shared_ptr<Command> command);
		void BindControllerUpCommand(int button, std::shared_ptr<Command> command);
		void BindKeyboardDownCommand(SDL_Scancode key, std::shared_ptr<Command> command);
		void BindControllerDownCommand(int button, std::shared_ptr<Command> command);

	/*	void HandleKeyboardInput(int key, float elapsedSec);
		void HandleControllerInput(int button, float elapsedSec);*/

		// Unbind keyboard commands
		void UnbindKeyboardCommand(SDL_Scancode input, std::shared_ptr<Command> command);
		void UnbindControllerCommand(int input, std::shared_ptr<Command> command);
		void UnbindKeyboardUpCommand(SDL_Scancode input, std::shared_ptr<Command> command);
		void UnbindControllerUpCommand(int input, std::shared_ptr<Command> command);
		void UnbindKeyboardDownCommand(SDL_Scancode input, std::shared_ptr<Command> command);
		void UnbindControllerDownCommand(int input, std::shared_ptr<Command> command);

		template<typename KeyType>
		void ProcessMappedInput(
			std::unordered_map<KeyType, std::vector<std::shared_ptr<Command>>>& commandMap,
			std::function<bool(KeyType)> isPressedFunc,
			float elapsedSec)
		{
			for (const auto& [input, commands] : commandMap) {
				if (isPressedFunc(input)) {
					for (auto& command : commands) {
						command->Execute(elapsedSec);
					}
				}
			}
		}
	private:
		Controller m_Controller = Controller(0);
		KeyBoardInput m_Keyboard = KeyBoardInput();

		// Map of inputs to commands
		std::unordered_map<SDL_Scancode, std::vector<std::shared_ptr<Command>>> m_keyboardCommandMap;
		std::unordered_map<int, std::vector<std::shared_ptr<Command>>> m_controllerCommandMap;
		std::unordered_map<SDL_Scancode, std::vector<std::shared_ptr<Command>>> m_keyboardUpCommandMap;
		std::unordered_map<int, std::vector<std::shared_ptr<Command>>> m_controllerUpCommandMap;
		std::unordered_map<SDL_Scancode, std::vector<std::shared_ptr<Command>>> m_keyboardDownCommandMap;
		std::unordered_map<int, std::vector<std::shared_ptr<Command>>> m_controllerDownCommandMap;
	};

}
