#pragma once
#include "SteamAchievement.h"

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

	class SteamAchievementObserver
		: public Observer
	{
	public:
		SteamAchievementObserver();
		void Notify(GameEvent event, GameObject* actor) override;
		
	};


}

