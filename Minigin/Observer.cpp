#include "Observer.h"


dae::SteamAchievementObserver::SteamAchievementObserver()
{
}

void dae::SteamAchievementObserver::Notify(GameEvent event, GameObject* )
	{
		switch (event) {
		case GameEvent::playerWon:
			if (g_SteamAchievements)
				g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
			break;
		}
	}
