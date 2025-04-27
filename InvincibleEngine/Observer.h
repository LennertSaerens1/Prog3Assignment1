#pragma once


namespace dae
{
	class GameObject;

	enum class GameEvent
	{
		PlayerDied,
		PlayerScored,
		playerWon
	}; //Change to be on game niveau, check slides again

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(GameEvent event, dae::GameObject* actor) = 0;
	};
}

