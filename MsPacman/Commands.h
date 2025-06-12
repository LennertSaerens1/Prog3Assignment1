#pragma once
#include "GameObject.h"
#include "PacManCharacters.h"
#include "Command.h"

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
		void Execute(float ) override
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
}

