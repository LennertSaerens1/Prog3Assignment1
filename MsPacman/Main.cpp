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
#include "ServiceLocator.h"
#include "SoundSystem.h" 
#include "Utils.h"
#include "GridComponent.h"
#include "GhostComponent.h"
#include "Utils.cpp"
#include "NameInputComponent.h"

void load();
void TwoPlayer();
void pvpScreen();

void titleScreen()
{
	dae::InputManager::GetInstance().ClearCommands();

	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SoundSystem>());

	auto& scene = dae::SceneManager::GetInstance().CreateScene("TitleScreen");
	dae::SceneManager::GetInstance().SetActiveScene("TitleScreen");


	auto font = dae::ResourceManager::GetInstance().LoadFont("PacManFont.ttf", 15);
	auto bigFont = dae::ResourceManager::GetInstance().LoadFont("PacManFont.ttf", 50);

	auto screen = std::make_shared<dae::GameObject>();
	screen->AddComponent<dae::RenderComponent>("HUD.png", *screen.get());
	screen->SetWorldPosition(0, 0);
	screen->getComponent<dae::RenderComponent>()->SetSrcRect(utils::Rect{ 0 , 8, 224, 288 });
	screen->getComponent<dae::RenderComponent>()->SetDestRect(utils::Rect{ 0, 0, 224 * 3, 288 * 3 });

	scene.Add(screen);

	int highScore = utils::LoadHighScore();

	auto highScoreDisplay = std::make_shared<dae::GameObject>();
	highScoreDisplay->AddComponent<dae::TextComponent>(std::to_string(highScore), font, *highScoreDisplay.get());
	highScoreDisplay->SetWorldPosition(300, 50);

	scene.Add(highScoreDisplay);

	auto titleText = std::make_shared<dae::GameObject>();
	titleText->AddComponent <dae::TextComponent>("Ms PacMan", bigFont, *titleText.get());

	titleText->SetWorldPosition(100, 200);
	scene.Add(titleText);

	auto singlePlayerText = std::make_shared<dae::GameObject>();
	singlePlayerText->AddComponent<dae::TextComponent>("Press A for Single Player", font, *singlePlayerText.get());
	singlePlayerText->SetWorldPosition(100, 500);
	scene.Add(singlePlayerText);

	auto twoPlayerText = std::make_shared<dae::GameObject>();
	twoPlayerText->AddComponent<dae::TextComponent>("Press B for Two Player", font, *twoPlayerText.get());
	twoPlayerText->SetWorldPosition(100, 550);
	scene.Add(twoPlayerText);

	auto pvpText = std::make_shared<dae::GameObject>();
	pvpText->AddComponent<dae::TextComponent>("Press X for PVP", font, *pvpText.get());
	pvpText->SetWorldPosition(100, 600);
	scene.Add(pvpText);

	auto creditsText = std::make_shared<dae::GameObject>();
	creditsText->AddComponent<dae::TextComponent>("Made by: Lennert Saerens", font, *creditsText.get());
	creditsText->SetWorldPosition(50, 870);
	scene.Add(creditsText);

	auto creditText2 = std::make_shared<dae::GameObject>();
	creditText2->AddComponent<dae::TextComponent>("2DAE-GD19", font, *creditText2.get());
	creditText2->SetWorldPosition(50, 900);
	scene.Add(creditText2);

	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_A, std::make_shared<dae::LoadSceneCommand>("singlePlayer", load));
	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_B, std::make_shared<dae::LoadSceneCommand>("TwoPlayer", TwoPlayer));

	dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_A, std::make_shared<dae::LoadSceneCommand>("singlePlayer", load));
	dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_B, std::make_shared<dae::LoadSceneCommand>("TwoPlayer", TwoPlayer));

	dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_X, std::make_shared<dae::LoadSceneCommand>("pvpScreen", pvpScreen));
	dae::InputManager::GetInstance().BindController2DownCommand(XINPUT_GAMEPAD_X, std::make_shared<dae::LoadSceneCommand>("pvpScreen", pvpScreen));

	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_F2, std::make_shared<dae::MuteCommand>());
}

void finalScreen()
{
	dae::InputManager::GetInstance().ClearCommands();

	auto& scene = dae::SceneManager::GetInstance().CreateScene("FinalScreen");
	dae::SceneManager::GetInstance().SetActiveScene("FinalScreen");
	auto font = dae::ResourceManager::GetInstance().LoadFont("PacManFont.ttf", 36);
	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("PacManFont.ttf", 15);

	auto scoreBoard = std::make_shared<dae::GameObject>();
	scoreBoard->AddComponent<dae::TextComponent>("Top scores:", font, *scoreBoard.get());
	scoreBoard->SetWorldPosition(100, 100);

	scene.Add(scoreBoard);

	auto highScores = utils::LoadAllHighScoreEntries();

	for (unsigned int i = 0; i < highScores.size(); ++i)
	{
		auto scoreText = std::make_shared<dae::GameObject>();
		scoreText->AddComponent<dae::TextComponent>(std::to_string(highScores[i].score), font, *scoreText.get());
		scoreText->SetWorldPosition(150.f, 150.f + i * 50);
		scene.Add(scoreText);

		auto nameText = std::make_shared<dae::GameObject>();
		std::string name = highScores[i].name;
		nameText->AddComponent<dae::TextComponent>(name, font, *nameText.get());
		nameText->SetWorldPosition(400.f, 150.f + i * 50);
		scene.Add(nameText);
	}

	auto backToTitle = std::make_shared<dae::GameObject>();
	backToTitle->AddComponent<dae::TextComponent>("Press start/enter to go back to title screen", smallFont, *backToTitle.get());
	backToTitle->SetWorldPosition(10, 800);
	scene.Add(backToTitle);

	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_RETURN, std::make_shared<dae::LoadSceneCommand>("TitleScreen", titleScreen));
	dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_START, std::make_shared<dae::LoadSceneCommand>("TitleScreen", titleScreen));

	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_F2, std::make_shared<dae::MuteCommand>());

}

void nameInputScene()
{
	dae::InputManager::GetInstance().ClearCommands();

	auto& scene = dae::SceneManager::GetInstance().CreateScene("NameInput");
	dae::SceneManager::GetInstance().SetActiveScene("NameInput");
	auto font = dae::ResourceManager::GetInstance().LoadFont("PacManFont.ttf", 36);
	auto inputText = std::make_shared<dae::GameObject>();
	inputText->AddComponent<dae::TextComponent>("Enter your name:", font, *inputText.get());
	inputText->SetWorldPosition(50, 100);
	scene.Add(inputText);
	// Add input field for name
	auto nameInput = std::make_shared<dae::GameObject>();
	nameInput->AddComponent<dae::NameInputComponent>(*nameInput.get(), font); // 10 characters max
	nameInput->SetWorldPosition(200, 300);
	scene.Add(nameInput);

	// Add submit button
	auto submitButton = std::make_shared<dae::GameObject>();
	submitButton->AddComponent<dae::TextComponent>("Press start to submit", font, *submitButton.get());
	submitButton->SetWorldPosition(50, 500);
	scene.Add(submitButton);

	// Bind submit action to start button
	dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_START, std::make_shared<dae::LoadSceneCommand>("FinalScreen", finalScreen));

	auto nameInputComp = nameInput->getComponent<dae::NameInputComponent>();
	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_DOWN, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::LetterDown));
	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_UP, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::LetterUp));
	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_LEFT, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::Left));
	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_RIGHT, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::Right));

	dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_DPAD_DOWN, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::LetterDown));
	dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_DPAD_UP, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::LetterUp));
	dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_DPAD_LEFT, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::Left));
	dae::InputManager::GetInstance().BindControllerDownCommand(XINPUT_GAMEPAD_DPAD_RIGHT, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::Right));

	dae::InputManager::GetInstance().BindController2DownCommand(XINPUT_GAMEPAD_DPAD_DOWN, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::LetterDown));
	dae::InputManager::GetInstance().BindController2DownCommand(XINPUT_GAMEPAD_DPAD_UP, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::LetterUp));
	dae::InputManager::GetInstance().BindController2DownCommand(XINPUT_GAMEPAD_DPAD_LEFT, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::Left));
	dae::InputManager::GetInstance().BindController2DownCommand(XINPUT_GAMEPAD_DPAD_RIGHT, std::make_shared<dae::NameInputCommand>(nameInputComp, dae::NameInputType::Right));

	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_F2, std::make_shared<dae::MuteCommand>());
}

void load()
{
	dae::InputManager::GetInstance().ClearCommands();
	
	auto& scene = dae::SceneManager::GetInstance().CreateScene("singlePlayer");
	dae::SceneManager::GetInstance().SetActiveScene("singlePlayer");

	auto font = dae::ResourceManager::GetInstance().LoadFont("PacManFont.ttf", 36);

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(std::string("SpriteSheet.png"),*go.get());
	go->SetWorldPosition(0, 100);
	auto renderComponent = go->getComponent<dae::RenderComponent>();
	renderComponent->SetSrcRect(utils::Rect{ 228, 0, 224, 248 });
	//renderComponent->SetSrcRect(utils::Rect{ 0, 248 *2, 224, 248 });
	renderComponent->SetDestRect(utils::Rect{0, 0, 224*3, 248*3});

	//scene.Add(go);

	//auto grid = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::GridComponent>(*go.get(), 28, 31, 224 * 3, 248 * 3);
	//grid->SetWorldPosition(0, 100);
	auto gridComponent = go->getComponent<dae::GridComponent>();
	gridComponent->LoadGridFromFile("level1.txt");

	scene.Add(go);

	{
		auto pacMan = std::make_shared<dae::GameObject>();
		auto pacManWorld = gridComponent->GetWorldCoordinatesMiddle(13, 23);
		pacMan->AddComponent<dae::PacManCharacter>(*pacMan.get(), gridComponent, nameInputScene);

		pacMan->getComponent<dae::PacManCharacter>()->SetMiddlePosition(pacManWorld.x, pacManWorld.y);

		gridComponent->GetPacMan(pacMan->getComponent<dae::PacManCharacter>());

		// Create commands for PacMan
		auto moveUp = std::make_shared<dae::MoveCommand>(dae::Direction::Up, pacMan.get());
		auto moveDown = std::make_shared<dae::MoveCommand>(dae::Direction::Down, pacMan.get());
		auto moveLeft = std::make_shared<dae::MoveCommand>(dae::Direction::Left, pacMan.get());
		auto moveRight = std::make_shared<dae::MoveCommand>(dae::Direction::Right, pacMan.get());

		// Bind PacMan movement to keyboard (WASD)
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, moveUp);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, moveDown);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, moveLeft);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, moveRight);
		
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP, moveUp);
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN, moveDown);
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT, moveLeft);
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT, moveRight);


		auto HighScoreText = std::make_shared<dae::GameObject>();
		HighScoreText->AddComponent<dae::TextComponent>("High Score:", font, *HighScoreText.get());
		HighScoreText->SetWorldPosition(200, 0);

		scene.Add(HighScoreText);

		int highScore = utils::LoadHighScore();

		auto highScoreDisplay = std::make_shared<dae::GameObject>();
		highScoreDisplay->AddComponent<dae::TextComponent>(std::to_string(highScore), font, *highScoreDisplay.get());
		highScoreDisplay->SetWorldPosition(300, 50);

		scene.Add(highScoreDisplay);

		{
			/*auto livesDisplayP1 = std::make_shared<dae::GameObject>();
			livesDisplayP1->AddComponent<dae::TextComponent>("# Lives: 3", smallFont, *livesDisplayP1.get());
			livesDisplayP1->SetWorldPosition(0, 100);
			auto livesDisplayObserver = new dae::LivesDisplayObserver(livesDisplayP1.get());
			pacMan->AddObserver(livesDisplayObserver);

			scene.Add(livesDisplayP1);*/

			auto lives = std::make_shared<dae::GameObject>();
			lives->AddComponent<dae::RenderComponent>("HUD.png", *lives.get());
			lives->SetWorldPosition(50, 248 * 3 + 100);
			auto renderComponentLives = lives->getComponent<dae::RenderComponent>();
			renderComponentLives->SetSrcRect(utils::Rect{ 696, 128, 80, 16 });
			renderComponentLives->SetDestRect(utils::Rect{ 0, 0, 80 * 3, 16 * 3 });

			auto livesDisplayObserver = new dae::LivesDisplayObserver(lives.get());
			pacMan->AddObserver(livesDisplayObserver);

			scene.Add(lives);

			auto scoreDisplayP1 = std::make_shared<dae::GameObject>();
			scoreDisplayP1->AddComponent<dae::TextComponent>("0", font, *scoreDisplayP1.get());
			scoreDisplayP1->SetWorldPosition(50, 50);
			auto scoreDisplayObserver = new dae::ScoreDisplayObserver(scoreDisplayP1.get());
			pacMan->AddObserver(scoreDisplayObserver);

			scene.Add(scoreDisplayP1);

			//auto damageCommand = std::make_shared<dae::DieCommand>(pacMan.get());
			//auto scorePelletCommand = std::make_shared<dae::AddScoreCommand>(100, pacMan.get());
			//auto scoreCommand200 = std::make_shared<dae::AddScoreCommand>(1000, pacMan.get());

		/*	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_X, damageCommand);
			dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_E, scoreCommand100);
			dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_Q, scoreCommand200);*/

			auto nextLevelCommand = std::make_shared<dae::NextLevelCommand>(pacMan.get(), go.get());

			dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_F1, nextLevelCommand);
			
			{
				auto blinky = std::make_shared<dae::GameObject>();
				blinky->AddComponent<dae::GhostComponent>(*blinky.get(), gridComponent, dae::GhostType::Blinky, 0.f);
				blinky->getComponent<dae::GhostComponent>()->AddEnemy(pacMan->getComponent<dae::PacManCharacter>());
				auto blinkyWorld = gridComponent->GetWorldCoordinatesMiddle(13, 11);

				blinky->getComponent<dae::GhostComponent>()->SetMiddlePosition(blinkyWorld.x, blinkyWorld.y);

				scene.Add(blinky);

				auto pinky = std::make_shared<dae::GameObject>();
				pinky->AddComponent<dae::GhostComponent>(*pinky.get(), gridComponent, dae::GhostType::Pinky, 5.f);
				pinky->getComponent<dae::GhostComponent>()->AddEnemy(pacMan->getComponent<dae::PacManCharacter>());
				auto pinkyWorld = gridComponent->GetWorldCoordinatesMiddle(11, 14);

				pinky->getComponent<dae::GhostComponent>()->SetMiddlePosition(pinkyWorld.x, pinkyWorld.y);

				scene.Add(pinky);

				auto inky = std::make_shared<dae::GameObject>();
				inky->AddComponent<dae::GhostComponent>(*inky.get(), gridComponent, dae::GhostType::Inky, 10.f);
				inky->getComponent<dae::GhostComponent>()->AddEnemy(pacMan->getComponent<dae::PacManCharacter>());
				auto inkyWorld = gridComponent->GetWorldCoordinatesMiddle(13, 14);

				inky->getComponent<dae::GhostComponent>()->SetMiddlePosition(inkyWorld.x, inkyWorld.y);

				scene.Add(inky);

				auto sue = std::make_shared<dae::GameObject>();
				sue->AddComponent<dae::GhostComponent>(*sue.get(), gridComponent, dae::GhostType::Sue, 15.f);
				sue->getComponent<dae::GhostComponent>()->AddEnemy(pacMan->getComponent<dae::PacManCharacter>());
				auto sueWorld = gridComponent->GetWorldCoordinatesMiddle(15, 14);

				sue->getComponent<dae::GhostComponent>()->SetMiddlePosition(sueWorld.x, sueWorld.y);

				scene.Add(sue);

				gridComponent->AddGhost(blinky->getComponent<dae::GhostComponent>());
				gridComponent->AddGhost(pinky->getComponent<dae::GhostComponent>());
				gridComponent->AddGhost(inky->getComponent<dae::GhostComponent>());
				gridComponent->AddGhost(sue->getComponent<dae::GhostComponent>());
			}//Adding ghosts
		}//Creating Lives and Score Display for PacMan
		scene.Add(pacMan);

	}//Creating PacMan and MsPacManµ

	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_F2, std::make_shared<dae::MuteCommand>());	
}

void TwoPlayer()
{
	dae::InputManager::GetInstance().ClearCommands();

	auto& scene = dae::SceneManager::GetInstance().CreateScene("TwoPlayer");
	dae::SceneManager::GetInstance().SetActiveScene("TwoPlayer");

	auto font = dae::ResourceManager::GetInstance().LoadFont("PacManFont.ttf", 36);

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(std::string("SpriteSheet.png"), *go.get());
	go->SetWorldPosition(0, 100);
	auto renderComponent = go->getComponent<dae::RenderComponent>();
	renderComponent->SetSrcRect(utils::Rect{ 228, 0, 224, 248 });
	//renderComponent->SetSrcRect(utils::Rect{ 0, 248 *2, 224, 248 });
	renderComponent->SetDestRect(utils::Rect{ 0, 0, 224 * 3, 248 * 3 });

	//scene.Add(go);

	//auto grid = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::GridComponent>(*go.get(), 28, 31, 224 * 3, 248 * 3);
	//grid->SetWorldPosition(0, 100);
	auto gridComponent = go->getComponent<dae::GridComponent>();
	gridComponent->LoadGridFromFile("level1.txt");

	scene.Add(go);

	{
		auto msPacMan = std::make_shared<dae::GameObject>();
		auto msPacManWorld = gridComponent->GetWorldCoordinatesMiddle(13, 23);
		msPacMan->AddComponent<dae::PacManCharacter>(*msPacMan.get(), gridComponent, nameInputScene);

		msPacMan->getComponent<dae::PacManCharacter>()->SetMiddlePosition(msPacManWorld.x, msPacManWorld.y);

		gridComponent->GetPacMan(msPacMan->getComponent<dae::PacManCharacter>());

		// Create commands for PacMan
		auto moveUp = std::make_shared<dae::MoveCommand>(dae::Direction::Up, msPacMan.get());
		auto moveDown = std::make_shared<dae::MoveCommand>(dae::Direction::Down, msPacMan.get());
		auto moveLeft = std::make_shared<dae::MoveCommand>(dae::Direction::Left, msPacMan.get());
		auto moveRight = std::make_shared<dae::MoveCommand>(dae::Direction::Right, msPacMan.get());

		// Bind PacMan movement to keyboard (WASD)
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, moveUp);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, moveDown);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, moveLeft);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, moveRight);

		dae::InputManager::GetInstance().BindController2Command(XINPUT_GAMEPAD_DPAD_UP, moveUp);
		dae::InputManager::GetInstance().BindController2Command(XINPUT_GAMEPAD_DPAD_DOWN, moveDown);
		dae::InputManager::GetInstance().BindController2Command(XINPUT_GAMEPAD_DPAD_LEFT, moveLeft);
		dae::InputManager::GetInstance().BindController2Command(XINPUT_GAMEPAD_DPAD_RIGHT, moveRight);

		auto pacMan = std::make_shared<dae::GameObject>();
		auto pacManWorld = gridComponent->GetWorldCoordinatesMiddle(14, 23);
		pacMan->AddComponent<dae::PacManCharacter>(*pacMan.get(), gridComponent,nullptr , true);

		pacMan->getComponent<dae::PacManCharacter>()->SetMiddlePosition(pacManWorld.x, pacManWorld.y);

		//Create Commands for pacMan
		auto pacUp = std::make_shared<dae::MoveCommand>(dae::Direction::Up, pacMan.get());
		auto pacDown = std::make_shared<dae::MoveCommand>(dae::Direction::Down, pacMan.get());
		auto pacLeft = std::make_shared<dae::MoveCommand>(dae::Direction::Left, pacMan.get());
		auto pacRight = std::make_shared<dae::MoveCommand>(dae::Direction::Right, pacMan.get());

		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_UP, pacUp);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_DOWN, pacDown);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_LEFT, pacLeft);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_RIGHT, pacRight);

		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP, pacUp);
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN, pacDown);
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT, pacLeft);
		dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT, pacRight);

		msPacMan->getComponent<dae::PacManCharacter>()->AddOtherPlayer(pacMan->getComponent<dae::PacManCharacter>());
		pacMan->getComponent<dae::PacManCharacter>()->AddOtherPlayer(msPacMan->getComponent<dae::PacManCharacter>());

		auto HighScoreText = std::make_shared<dae::GameObject>();
		HighScoreText->AddComponent<dae::TextComponent>("High Score:", font, *HighScoreText.get());
		HighScoreText->SetWorldPosition(200, 0);

		scene.Add(HighScoreText);

		int highScore = utils::LoadHighScore();

		auto highScoreDisplay = std::make_shared<dae::GameObject>();
		highScoreDisplay->AddComponent<dae::TextComponent>(std::to_string(highScore), font, *highScoreDisplay.get());
		highScoreDisplay->SetWorldPosition(300, 50);

		scene.Add(highScoreDisplay);

		{
			/*auto livesDisplayP1 = std::make_shared<dae::GameObject>();
			livesDisplayP1->AddComponent<dae::TextComponent>("# Lives: 3", smallFont, *livesDisplayP1.get());
			livesDisplayP1->SetWorldPosition(0, 100);
			auto livesDisplayObserver = new dae::LivesDisplayObserver(livesDisplayP1.get());
			pacMan->AddObserver(livesDisplayObserver);

			scene.Add(livesDisplayP1);*/

			auto lives = std::make_shared<dae::GameObject>();
			lives->AddComponent<dae::RenderComponent>("HUD.png", *lives.get());
			lives->SetWorldPosition(50, 248 * 3 + 100);
			auto renderComponentLives = lives->getComponent<dae::RenderComponent>();
			renderComponentLives->SetSrcRect(utils::Rect{ 696, 128, 80, 16 });
			renderComponentLives->SetDestRect(utils::Rect{ 0, 0, 80 * 3, 16 * 3 });

			auto livesDisplayObserver = new dae::LivesDisplayObserver(lives.get());
			msPacMan->AddObserver(livesDisplayObserver);

			scene.Add(lives);

			auto scoreDisplayP1 = std::make_shared<dae::GameObject>();
			scoreDisplayP1->AddComponent<dae::TextComponent>("0", font, *scoreDisplayP1.get());
			scoreDisplayP1->SetWorldPosition(50, 50);
			auto scoreDisplayObserver = new dae::ScoreDisplayObserver(scoreDisplayP1.get());
			msPacMan->AddObserver(scoreDisplayObserver);

			scene.Add(scoreDisplayP1);


			auto nextLevelCommand = std::make_shared<dae::NextLevelCommand>(msPacMan.get(), go.get());

			dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_F1, nextLevelCommand);

			{
				auto blinky = std::make_shared<dae::GameObject>();
				blinky->AddComponent<dae::GhostComponent>(*blinky.get(), gridComponent, dae::GhostType::Blinky, 0.f);
				blinky->getComponent<dae::GhostComponent>()->AddEnemy(pacMan->getComponent<dae::PacManCharacter>());
				blinky->getComponent<dae::GhostComponent>()->AddEnemy(msPacMan->getComponent<dae::PacManCharacter>());

				auto blinkyWorld = gridComponent->GetWorldCoordinatesMiddle(13, 11);

				blinky->getComponent<dae::GhostComponent>()->SetMiddlePosition(blinkyWorld.x, blinkyWorld.y);

				scene.Add(blinky);

				auto pinky = std::make_shared<dae::GameObject>();
				pinky->AddComponent<dae::GhostComponent>(*pinky.get(), gridComponent, dae::GhostType::Pinky, 5.f);
				pinky->getComponent<dae::GhostComponent>()->AddEnemy(pacMan->getComponent<dae::PacManCharacter>());
				pinky->getComponent<dae::GhostComponent>()->AddEnemy(msPacMan->getComponent<dae::PacManCharacter>());

				auto pinkyWorld = gridComponent->GetWorldCoordinatesMiddle(12, 14);

				pinky->getComponent<dae::GhostComponent>()->SetMiddlePosition(pinkyWorld.x, pinkyWorld.y);

				scene.Add(pinky);

				auto inky = std::make_shared<dae::GameObject>();
				inky->AddComponent<dae::GhostComponent>(*inky.get(), gridComponent, dae::GhostType::Inky, 10.f);
				inky->getComponent<dae::GhostComponent>()->AddEnemy(pacMan->getComponent<dae::PacManCharacter>());
				inky->getComponent<dae::GhostComponent>()->AddEnemy(msPacMan->getComponent<dae::PacManCharacter>());

				auto inkyWorld = gridComponent->GetWorldCoordinatesMiddle(13, 14);

				inky->getComponent<dae::GhostComponent>()->SetMiddlePosition(inkyWorld.x, inkyWorld.y);

				scene.Add(inky);

				auto sue = std::make_shared<dae::GameObject>();
				sue->AddComponent<dae::GhostComponent>(*sue.get(), gridComponent, dae::GhostType::Sue, 15.f);
				sue->getComponent<dae::GhostComponent>()->AddEnemy(pacMan->getComponent<dae::PacManCharacter>());
				sue->getComponent<dae::GhostComponent>()->AddEnemy(msPacMan->getComponent<dae::PacManCharacter>());

				auto sueWorld = gridComponent->GetWorldCoordinatesMiddle(14, 14);

				sue->getComponent<dae::GhostComponent>()->SetMiddlePosition(sueWorld.x, sueWorld.y);

				scene.Add(sue);

				gridComponent->AddGhost(blinky->getComponent<dae::GhostComponent>());
				gridComponent->AddGhost(pinky->getComponent<dae::GhostComponent>());
				gridComponent->AddGhost(inky->getComponent<dae::GhostComponent>());
				gridComponent->AddGhost(sue->getComponent<dae::GhostComponent>());
			}//Adding ghosts

		}//Creating Lives and Score Display for PacMan
		scene.Add(msPacMan);
		scene.Add(pacMan);

	}//Creating PacMan and MsPacMan

	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_F2, std::make_shared<dae::MuteCommand>());

}

void pvpScreen()
{
	dae::InputManager::GetInstance().ClearCommands();

	auto& scene = dae::SceneManager::GetInstance().CreateScene("pvpScreen");
	dae::SceneManager::GetInstance().SetActiveScene("pvpScreen");

	auto font = dae::ResourceManager::GetInstance().LoadFont("PacManFont.ttf", 36);

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(std::string("SpriteSheet.png"), *go.get());
	go->SetWorldPosition(0, 100);
	auto renderComponent = go->getComponent<dae::RenderComponent>();
	renderComponent->SetSrcRect(utils::Rect{ 228, 0, 224, 248 });
	//renderComponent->SetSrcRect(utils::Rect{ 0, 248 *2, 224, 248 });
	renderComponent->SetDestRect(utils::Rect{ 0, 0, 224 * 3, 248 * 3 });

	//scene.Add(go);

	//auto grid = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::GridComponent>(*go.get(), 28, 31, 224 * 3, 248 * 3);
	//grid->SetWorldPosition(0, 100);
	auto gridComponent = go->getComponent<dae::GridComponent>();
	gridComponent->LoadGridFromFile("level1.txt");

	scene.Add(go);

	{
		auto pacMan = std::make_shared<dae::GameObject>();
		auto pacManWorld = gridComponent->GetWorldCoordinatesMiddle(13, 23);
		pacMan->AddComponent<dae::PacManCharacter>(*pacMan.get(), gridComponent, titleScreen);

		pacMan->getComponent<dae::PacManCharacter>()->SetMiddlePosition(pacManWorld.x, pacManWorld.y);

		gridComponent->GetPacMan(pacMan->getComponent<dae::PacManCharacter>());

		// Create commands for PacMan
		auto moveUp = std::make_shared<dae::MoveCommand>(dae::Direction::Up, pacMan.get());
		auto moveDown = std::make_shared<dae::MoveCommand>(dae::Direction::Down, pacMan.get());
		auto moveLeft = std::make_shared<dae::MoveCommand>(dae::Direction::Left, pacMan.get());
		auto moveRight = std::make_shared<dae::MoveCommand>(dae::Direction::Right, pacMan.get());

		// Bind PacMan movement to keyboard (WASD)
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_W, moveUp);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_S, moveDown);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_A, moveLeft);
		dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_D, moveRight);

		dae::InputManager::GetInstance().BindController2Command(XINPUT_GAMEPAD_DPAD_UP, moveUp);
		dae::InputManager::GetInstance().BindController2Command(XINPUT_GAMEPAD_DPAD_DOWN, moveDown);
		dae::InputManager::GetInstance().BindController2Command(XINPUT_GAMEPAD_DPAD_LEFT, moveLeft);
		dae::InputManager::GetInstance().BindController2Command(XINPUT_GAMEPAD_DPAD_RIGHT, moveRight);


		auto HighScoreText = std::make_shared<dae::GameObject>();
		HighScoreText->AddComponent<dae::TextComponent>("High Score:", font, *HighScoreText.get());
		HighScoreText->SetWorldPosition(200, 0);

		scene.Add(HighScoreText);

		int highScore = utils::LoadHighScore();

		auto highScoreDisplay = std::make_shared<dae::GameObject>();
		highScoreDisplay->AddComponent<dae::TextComponent>(std::to_string(highScore), font, *highScoreDisplay.get());
		highScoreDisplay->SetWorldPosition(300, 50);

		scene.Add(highScoreDisplay);

		{
			/*auto livesDisplayP1 = std::make_shared<dae::GameObject>();
			livesDisplayP1->AddComponent<dae::TextComponent>("# Lives: 3", smallFont, *livesDisplayP1.get());
			livesDisplayP1->SetWorldPosition(0, 100);
			auto livesDisplayObserver = new dae::LivesDisplayObserver(livesDisplayP1.get());
			pacMan->AddObserver(livesDisplayObserver);

			scene.Add(livesDisplayP1);*/

			auto lives = std::make_shared<dae::GameObject>();
			lives->AddComponent<dae::RenderComponent>("HUD.png", *lives.get());
			lives->SetWorldPosition(50, 248 * 3 + 100);
			auto renderComponentLives = lives->getComponent<dae::RenderComponent>();
			renderComponentLives->SetSrcRect(utils::Rect{ 696, 128, 80, 16 });
			renderComponentLives->SetDestRect(utils::Rect{ 0, 0, 80 * 3, 16 * 3 });

			auto livesDisplayObserver = new dae::LivesDisplayObserver(lives.get());
			pacMan->AddObserver(livesDisplayObserver);

			scene.Add(lives);

			auto scoreDisplayP1 = std::make_shared<dae::GameObject>();
			scoreDisplayP1->AddComponent<dae::TextComponent>("0", font, *scoreDisplayP1.get());
			scoreDisplayP1->SetWorldPosition(50, 50);
			auto scoreDisplayObserver = new dae::ScoreDisplayObserver(scoreDisplayP1.get());
			pacMan->AddObserver(scoreDisplayObserver);

			scene.Add(scoreDisplayP1);

			auto nextLevelCommand = std::make_shared<dae::NextLevelCommand>(pacMan.get(), go.get());

			dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_F1, nextLevelCommand);

			{
				auto playerGhost = std::make_shared<dae::GameObject>();
				playerGhost->AddComponent<dae::GhostComponent>(*playerGhost.get(), gridComponent, dae::GhostType::Player, 0.f);
				playerGhost->getComponent<dae::GhostComponent>()->AddEnemy(pacMan->getComponent<dae::PacManCharacter>());

				auto blinkyWorld = gridComponent->GetWorldCoordinatesMiddle(13, 11);

				playerGhost->getComponent<dae::GhostComponent>()->SetMiddlePosition(blinkyWorld.x, blinkyWorld.y);

				scene.Add(playerGhost);

				gridComponent->AddGhost(playerGhost->getComponent<dae::GhostComponent>());

				// Create commands for ghost
				auto ghostMoveUp = std::make_shared<dae::MoveGhostCommand>(dae::Direction::Up, playerGhost.get());
				auto ghostMoveDown = std::make_shared<dae::MoveGhostCommand>(dae::Direction::Down, playerGhost.get());
				auto ghostMoveLeft = std::make_shared<dae::MoveGhostCommand>(dae::Direction::Left, playerGhost.get());
				auto ghostMoveRight = std::make_shared<dae::MoveGhostCommand>(dae::Direction::Right, playerGhost.get());

				// Bind PacMan movement to keyboard (WASD)
				dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_UP, ghostMoveUp);
				dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_DOWN, ghostMoveDown);
				dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_LEFT, ghostMoveLeft);
				dae::InputManager::GetInstance().BindKeyboardCommand(SDL_SCANCODE_RIGHT, ghostMoveRight);

				dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP, ghostMoveUp);
				dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN, ghostMoveDown);
				dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT, ghostMoveLeft);
				dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT, ghostMoveRight);

			}//Adding ghosts
		}//Creating Lives and Score Display for PacMan
		scene.Add(pacMan);

	}//Creating PacMan and MsPacManµ

	dae::InputManager::GetInstance().BindKeyboardDownCommand(SDL_SCANCODE_F2, std::make_shared<dae::MuteCommand>());

}



int main(int, char*[]) {
	dae::InvincibleEngine engine("../Data/");
	engine.SetFrameRate(60);
	engine.Run(titleScreen);
    return 0;
}