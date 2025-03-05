#pragma once
#include "Singleton.h"
#include "Controller.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput(float deltaTime);
	private:
		Controller m_Controller = Controller(1);
	};

}
