#pragma once
#include "GameObject.h"
#include "PacManCharacters.h"

namespace dae {
	class Command {
	public:
		virtual ~Command() = default;
		virtual void Execute(float elapsedSec) = 0;
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
		MoveCommand(float speed, glm::vec2 dir, GameObject* actor)
			: GameObjectCommand(actor)
			, m_movementSpeed{ speed }
			, m_direction{ dir }
		{
		}
		void Execute(float elapsedSec) override
		{
			auto owner = GetGameObject();
			owner->AddWorldOffset(glm::vec3(m_direction.x * m_movementSpeed * elapsedSec, m_direction.y * m_movementSpeed * elapsedSec, 0));
		}
	private:
		float m_movementSpeed;
		glm::vec2 m_direction;
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

