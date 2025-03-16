#pragma once
class GameObject;

namespace dae
{
	enum class GameEvent
	{
		PlayerDied,
		PlayerScored,
		playerWon
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(GameEvent event, GameObject* actor) = 0;
	};

	class SteamAchievements : public Observer
	{
	public:
		void Notify(GameEvent event, GameObject* actor) override;
		
	};


}

