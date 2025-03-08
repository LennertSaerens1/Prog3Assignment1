#pragma once
#include "Singleton.h"
#include "Controller.h"
#include "KeyBoardInput.h"
#include <unordered_map>
#include "Commands.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);

		// Bind keys/buttons to commands
		void BindKeyboardCommand(int key, std::shared_ptr<Command> command);
		void BindControllerCommand(int button, std::shared_ptr<Command> command);

		void HandleKeyboardInput(int key, float elapsedSec);
		void HandleControllerInput(int button, float elapsedSec);
	private:
		Controller m_Controller = Controller(0);
		KeyBoardInput m_Keyboard = KeyBoardInput();

		// Map of inputs to commands
		std::unordered_map<int, std::vector<std::shared_ptr<Command>>> m_keyboardCommandMap;
		std::unordered_map<int, std::vector<std::shared_ptr<Command>>> m_controllerCommandMap;
	};

}
