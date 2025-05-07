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
				pacMan->SetState(PacManState::m_upState.get());
				break;
			case dae::Down:
				pacMan->SetState(PacManState::m_downState.get());				
				break;
			case dae::Left:
				pacMan->SetState(PacManState::m_leftState.get());			
				break;
			case dae::Right:
				pacMan->SetState(PacManState::m_rightState.get());				
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
}

