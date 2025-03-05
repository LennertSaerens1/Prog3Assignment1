#pragma once
#include <Windows.h>
#include <Xinput.h>

namespace dae
{
	class Controller
	{
	public:
		Controller(int controllerIndex);
		void ProcessInput();
		bool IsDownThisFrame(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsPressed(unsigned int button) const;
	private:

		DWORD m_controllerIndex;

		XINPUT_STATE currentState;
		XINPUT_STATE previousState;
		unsigned int buttonsPressedThisFrame;
		unsigned int buttonsReleasedThisFrame;
	};
}

