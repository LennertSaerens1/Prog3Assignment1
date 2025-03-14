#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <Windows.h>
#include<Xinput.h>

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "FpsComponent.h"
#include "RotationComponent.h"
#include "ImGuiComponent.h"
#include "MoveableComponent.h"
#include "Commands.h"
#include "InputManager.h"

void load()
{
	
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(std::string("background.tga"),*go.get());
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(std::string("logo.tga"),*go.get());
	go->SetWorldPosition(216, 200);
	scene.Add(go);
	
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextComponent>("Programming 4 Assignment", font, *to.get());
	to->SetWorldPosition(80, 40);
	scene.Add(to);

	auto fps = std::make_shared<dae::GameObject>();
	fps->AddComponent<dae::FpsComponent>(font,*fps.get());
	fps->SetWorldPosition(0, 0);
	scene.Add(fps);


	auto pacMan = std::make_shared<dae::GameObject>();
	pacMan->SetWorldPosition(50, 50);
	pacMan->AddComponent<dae::MoveableComponent>("PacMan.png", *pacMan.get(), 30.f);
	scene.Add(pacMan);

	// Create commands for PacMan
	auto moveUp = std::make_shared<dae::MoveUp>(pacMan.get());
	auto moveDown = std::make_shared<dae::MoveDown>(pacMan.get());
	auto moveLeft = std::make_shared<dae::MoveLeft>(pacMan.get());
	auto moveRight = std::make_shared<dae::MoveRight>(pacMan.get());

	// Bind PacMan movement to keyboard (WASD)
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, moveUp);
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, moveDown);
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, moveLeft);
	dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, moveRight);

	auto msPacMan = std::make_shared<dae::GameObject>();
	msPacMan->SetWorldPosition(50, 60);
	msPacMan->AddComponent<dae::MoveableComponent>("msPacMan.png", *msPacMan.get(), 60.f);
	scene.Add(msPacMan);

	// Create commands for MsPacMan
	auto msMoveUp = std::make_shared<dae::MoveUp>(msPacMan.get());
	auto msMoveDown = std::make_shared<dae::MoveDown>(msPacMan.get());
	auto msMoveLeft = std::make_shared<dae::MoveLeft>(msPacMan.get());
	auto msMoveRight = std::make_shared<dae::MoveRight>(msPacMan.get());

	// Bind MsPacMan movement to controller dpad
	dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP, msMoveUp);
	dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN, msMoveDown);
	dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT, msMoveLeft);
	dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT, msMoveRight);

	/*auto imGui = std::make_shared<dae::GameObject>();
	imGui->AddComponent<dae::ImGuiComponent>(*imGui.get());
	scene.Add(imGui);*/
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.SetFrameRate(165);
	engine.Run(load);
    return 0;
}