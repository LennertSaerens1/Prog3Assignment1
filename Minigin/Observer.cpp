#include "Observer.h"

void dae::SteamAchievements::Notify(GameEvent event, GameObject*)
{
	switch (event) {
	case GameEvent::playerWon:
		//Implement steam achievement
		break;
	}
}
