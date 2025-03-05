#include <SDL.h>
#include "iostream"
#include "InputManager.h"
#include "imgui_impl_sdl2.h"

bool dae::InputManager::ProcessInput(float)
{
	m_Controller.ProcessInput();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	if (m_Controller.IsDownThisFrame(XINPUT_GAMEPAD_A))
	{
		std::cout << "A button down";
	}
	else if (m_Controller.IsUpThisFrame(XINPUT_GAMEPAD_A))
	{
		std::cout << "A button up";
	}

	return true;
}
