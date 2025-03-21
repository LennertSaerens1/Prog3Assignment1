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
#include "Commands.h"
#include "InputManager.h"
#include "PacManCharacters.h"
#include "Displays.h"

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
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextComponent>("Programming 4 Assignment", font, *to.get());
	to->SetWorldPosition(80, 40);
	scene.Add(to);

	auto fps = std::make_shared<dae::GameObject>();
	fps->AddComponent<dae::FpsComponent>(font,*fps.get());
	fps->SetWorldPosition(0, 0);
	scene.Add(fps);

	auto info = std::make_shared<dae::GameObject>();
	info->AddComponent<dae::TextComponent>("WASD to move Pac-Man, Q and E to pick up pellets and X to die", smallFont, *info.get());
	info->SetWorldPosition(0, 80);
	scene.Add(info);
	auto infoP2 = std::make_shared<dae::GameObject>();
	infoP2->AddComponent<dae::TextComponent>("D-PAD to move Ms Pac-Man, Button X and B to pick up pellets and Button A to die", smallFont, *info.get());
	infoP2->SetWorldPosition(0, 150);
	scene.Add(infoP2);

	{
		auto pacMan = std::make_shared<dae::GameObject>();
		pacMan->SetWorldPosition(50, 50);
		pacMan->AddComponent<dae::PacManCharacter>(*pacMan.get());

		// Create commands for PacMan
		auto moveUp = std::make_shared<dae::MoveCommand>(30.f, glm::vec2(0.f, -1.f), pacMan.get());
		auto moveDown = std::make_shared<dae::MoveCommand>(30.f, glm::vec2(0.f, 1.f), pacMan.get());
		auto moveLeft = std::make_shared<dae::MoveCommand>(30.f, glm::vec2(-1.f, 0.f), pacMan.get());
		auto moveRight = std::make_shared<dae::MoveCommand>(30.f, glm::vec2(1.f, 0.f), pacMan.get());

		// Bind PacMan movement to keyboard (WASD)
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, moveUp);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, moveDown);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, moveLeft);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, moveRight);

		auto msPacMan = std::make_shared<dae::GameObject>();
		msPacMan->SetWorldPosition(50, 60);
		msPacMan->AddComponent<dae::PacManCharacter>(*msPacMan.get(),false);
		

		// Create commands for MsPacMan
		auto msMoveUp = std::make_shared<dae::MoveCommand>(60.f, glm::vec2(0.f, -1.f), msPacMan.get());
		auto msMoveDown = std::make_shared<dae::MoveCommand>(60.f, glm::vec2(0.f, 1.f), msPacMan.get());
		auto msMoveLeft = std::make_shared<dae::MoveCommand>(60.f, glm::vec2(-1.f, 0.f), msPacMan.get());
		auto msMoveRight = std::make_shared<dae::MoveCommand>(60.f, glm::vec2(1.f, 0.f), msPacMan.get());

		// Bind MsPacMan movement to controller dpad
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP, msMoveUp);
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN, msMoveDown);
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT, msMoveLeft);
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT, msMoveRight);
	

		{
			auto livesDisplayP1 = std::make_shared<dae::GameObject>();
			livesDisplayP1->AddComponent<dae::TextComponent>("# Lives: 3", smallFont, *livesDisplayP1.get());
			livesDisplayP1->SetWorldPosition(0, 100);
			auto livesDisplayObserver = std::make_shared<dae::LivesDisplayObserver>(livesDisplayP1.get());
			pacMan->AddObserver(livesDisplayObserver);

			scene.Add(livesDisplayP1);

			auto scoreDisplayP1 = std::make_shared<dae::GameObject>();
			scoreDisplayP1->AddComponent<dae::TextComponent>("Score: 0", smallFont, *scoreDisplayP1.get());
			scoreDisplayP1->SetWorldPosition(0, 120);
			auto scoreDisplayObserver = std::make_shared<dae::ScoreDisplayObserver>(scoreDisplayP1.get());
			pacMan->AddObserver(scoreDisplayObserver);

			scene.Add(scoreDisplayP1);

			auto damageCommand = std::make_shared<dae::DieCommand>(pacMan.get());
			auto scoreCommand100 = std::make_shared<dae::AddScoreCommand>(100, pacMan.get());
			auto scoreCommand200 = std::make_shared<dae::AddScoreCommand>(1000, pacMan.get());

			dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_X, damageCommand);
			dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_E, scoreCommand100);
			dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_Q, scoreCommand200);
			//PacMan Created

			auto livesDisplayP2 = std::make_shared<dae::GameObject>();
			livesDisplayP2->AddComponent<dae::TextComponent>("# Lives: 3", smallFont, *livesDisplayP2.get());
			livesDisplayP2->SetWorldPosition(0, 170);
			auto livesDisplayObserverP2 = std::make_shared<dae::LivesDisplayObserver>(livesDisplayP2.get());
			msPacMan->AddObserver(livesDisplayObserverP2);

			scene.Add(livesDisplayP2);

			auto scoreDisplayP2 = std::make_shared<dae::GameObject>();
			scoreDisplayP2->AddComponent<dae::TextComponent>("Score: 0", smallFont, *scoreDisplayP2.get());
			scoreDisplayP2->SetWorldPosition(0, 190);
			auto scoreDisplayObserverP2 = std::make_shared<dae::ScoreDisplayObserver>(scoreDisplayP2.get());
			msPacMan->AddObserver(scoreDisplayObserverP2);

			scene.Add(scoreDisplayP2);

			auto damageCommandP2 = std::make_shared<dae::DieCommand>(msPacMan.get());
			auto scoreCommand100P2 = std::make_shared<dae::AddScoreCommand>(100, msPacMan.get());
			auto scoreCommand200P2 = std::make_shared<dae::AddScoreCommand>(1000, msPacMan.get());
			
			dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_A, damageCommandP2);
			dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_B, scoreCommand100P2);
			dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_X, scoreCommand200P2);

		}//Creating Lives and Score Display for PacMan
		scene.Add(pacMan);
		scene.Add(msPacMan);
	}//Creating PacMan and MsPacMan
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.SetFrameRate(165);
	engine.Run(load);
    return 0;
}