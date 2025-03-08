#pragma once
#include "GameObject.h"
#include "MoveableComponent.h"

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

	class MoveLeft : public GameObjectCommand {
	public:
		MoveLeft(GameObject* actor) : GameObjectCommand(actor) {}
		void Execute(float elapsedSec) override
		{
			auto moveComponent = GetGameObject()->getComponent<MoveableComponent>();
			moveComponent->MoveLeft(elapsedSec);
		}
	};
	
	class MoveRight : public GameObjectCommand {
	public:
		MoveRight(GameObject* actor) : GameObjectCommand(actor) {}
		void Execute(float elapsedSec) override
		{
			auto moveComponent = GetGameObject()->getComponent<MoveableComponent>();
			moveComponent->MoveRight(elapsedSec);
		}
	};
	
	class MoveUp : public GameObjectCommand {
	public:
		MoveUp(GameObject* actor) : GameObjectCommand(actor) {}
		void Execute(float elapsedSec) override
		{
			auto moveComponent = GetGameObject()->getComponent<MoveableComponent>();
			moveComponent->MoveUp(elapsedSec);
		}
	};
	
	class MoveDown : public GameObjectCommand {
	public:
		MoveDown(GameObject* actor) : GameObjectCommand(actor) {}
		void Execute(float elapsedSec) override
		{
			auto moveComponent = GetGameObject()->getComponent<MoveableComponent>();
			moveComponent->MoveDown(elapsedSec);
		}
	};
}

