#pragma once
#include "GameObject.h"
#include "PacManCharacters.h"
#include "Command.h"
#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "NameInputComponent.h"
#include "ServiceLocator.h"
#include "GhostComponent.h"
#include <functional>

namespace dae {
	enum Direction
	{
		Up,
		Down,
		Left,
		Right
	};

	class GameObjectCommand : public Command {
		GameObject* m_actor;
	protected:
		GameObject* GetGameObject() const { return m_actor; }
	public:
		GameObjectCommand(GameObject* actor);
		virtual ~GameObjectCommand() = default;
	};

	class MoveCommand : public GameObjectCommand {
	public:
		MoveCommand(Direction dir, GameObject* actor)
			: GameObjectCommand(actor)
			, m_direction{ dir }
		{
		}
		void Execute(float) override
		{
			auto owner = GetGameObject();
			auto pacMan = owner->getComponent<PacManCharacter>();

			switch (m_direction)
			{
			case dae::Up:
				if (!pacMan->CanMoveUp()) return;
				if (!pacMan->GetIsMale())
					pacMan->SetState(PacManState::m_movingState.get());
				else
					pacMan->SetState(PacManState::m_mrMovingState.get());

				pacMan->SetMovementDirection(glm::vec3(0, -1, 0));
				break;
			case dae::Down:
				if (!pacMan->CanMoveDown()) return;
				if (!pacMan->GetIsMale())
					pacMan->SetState(PacManState::m_movingState.get());
				else
					pacMan->SetState(PacManState::m_mrMovingState.get());
				pacMan->SetMovementDirection(glm::vec3(0, 1, 0));
				break;
			case dae::Left:
				if (!pacMan->CanMoveLeft()) return;
				if (!pacMan->GetIsMale())
					pacMan->SetState(PacManState::m_movingState.get());
				else
					pacMan->SetState(PacManState::m_mrMovingState.get());
				pacMan->SetMovementDirection(glm::vec3(-1, 0, 0));
				break;
			case dae::Right:
				if (!pacMan->CanMoveRight()) return;
				if (!pacMan->GetIsMale())
					pacMan->SetState(PacManState::m_movingState.get());
				else
					pacMan->SetState(PacManState::m_mrMovingState.get());
				pacMan->SetMovementDirection(glm::vec3(1, 0, 0));
				break;
			default:
				break;
			}
		}
	private:
		Direction m_direction;
	};

	class DieCommand : public GameObjectCommand {
	public:
		DieCommand(GameObject* actor)
			: GameObjectCommand(actor)
		{
		}
		void Execute(float) override
		{
			auto owner = GetGameObject()->getComponent<PacManCharacter>();
			owner->DecreaseLives();
		}
	};

	class AddScoreCommand : public GameObjectCommand {
	public:
		AddScoreCommand(int score, GameObject* actor)
			: GameObjectCommand(actor)
			, m_score{ score }
		{
		}
		void Execute(float) override
		{
			auto owner = GetGameObject()->getComponent<PacManCharacter>();
			owner->AddScore(m_score);
		}
	private:
		int m_score;
	};

	class NextLevelCommand : public GameObjectCommand
	{
	public:
		NextLevelCommand(GameObject* actor, GameObject* grid)
			: GameObjectCommand(actor)
		{
			m_grid = grid;
		}
		void Execute(float) override
		{
			auto owner = GetGameObject()->getComponent<PacManCharacter>();
			owner->LevelUp();

			m_grid->getComponent<GridComponent>()->NextLevel();
		}
	private:
		GameObject* m_grid{ nullptr };
	};

	class LoadSceneCommand : public Command
	{
	public:
		LoadSceneCommand(const std::string& sceneName, const std::function<void()>& loadFunction)
			: m_SceneName(sceneName), m_LoadFunction(loadFunction) {
		}

		void Execute(float) override
		{
			// Clear scene properly to avoid dangling references
			auto& sceneManager = SceneManager::GetInstance();
			// Allow current scene to clean up properly
			//InputManager::GetInstance().ClearCommands();
			if (auto activeScene = sceneManager.GetActiveScene()) {
				activeScene->RemoveAll();
			}
			sceneManager.ClearScenes();

			// Store the function to be called later instead of calling it directly
			SceneManager::GetInstance().QueueSceneLoad(m_SceneName, m_LoadFunction);
		}

	private:
		std::string m_SceneName;
		std::function<void()> m_LoadFunction;
	};

	enum class NameInputType
	{
		Left,
		Right,
		LetterUp,
		LetterDown,
	};

	class NameInputCommand : public Command
	{
	public:
		NameInputCommand(NameInputComponent* comp, NameInputType type)
			: m_comp(comp)
			, m_type(type)
		{
		}
		void Execute(float) override
		{
			switch (m_type)
			{
			case dae::NameInputType::Left:
				m_comp->goLeft();
				break;
			case dae::NameInputType::Right:
				m_comp->goRight();
				break;
			case dae::NameInputType::LetterUp:
				m_comp->numberUp();
				break;
			case dae::NameInputType::LetterDown:
				m_comp->numberDown();
				break;
			default:
				break;
			}
		}
	private:
		NameInputComponent* m_comp{ nullptr };
		NameInputType m_type;
	};

	class MuteCommand : public Command
	{
	public:
		MuteCommand()
		{
		}

		void Execute(float) override
		{
			auto& base = dae::ServiceLocator::GetSoundSystem();
			if (auto* soundSystem = dynamic_cast<dae::SoundSystem*>(&base)) {
				soundSystem->SetMuted(!soundSystem->IsMuted());
			}
		}
	};

	class MoveGhostCommand : public Command
	{
	public:
		MoveGhostCommand(Direction dir, GameObject* ghost)
			: m_ghost(ghost)
			, m_direction(dir)
		{
		}
		void Execute(float) override
		{
			auto ghostComponent = m_ghost->getComponent<GhostComponent>();
			if (!ghostComponent) return;
			switch (m_direction)
			{
			case dae::Up:
				if (!ghostComponent->CanMoveUp()) return;
				ghostComponent->SetMovementDirection(glm::vec3(0, -1, 0));
				break;
			case dae::Down:
				if (!ghostComponent->CanMoveDown()) return;
				ghostComponent->SetMovementDirection(glm::vec3(0, 1, 0));
				break;
			case dae::Left:
				if (!ghostComponent->CanMoveLeft()) return;
				ghostComponent->SetMovementDirection(glm::vec3(-1, 0, 0));
				break;
			case dae::Right:
				if (!ghostComponent->CanMoveRight()) return;
				ghostComponent->SetMovementDirection(glm::vec3(1, 0, 0));
				break;
			default:
				break;
			}
		}
	private:
		GameObject* m_ghost{ nullptr };
		Direction m_direction{ Direction::Up
		};

	};
}

