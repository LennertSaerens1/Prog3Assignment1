#include "Displays.h"
#include "PacManCharacters.h"
#include "GameObject.h"

void dae::LivesDisplayObserver::Notify(GameEvent event, GameObject* actor)
{
	switch (event)
	{
	case GameEvent::PlayerDied:
		auto lives = actor->getComponent<PacManCharacter>()->GetLives();
		m_livesTextComp->SetText("Lives: " + std::to_string(lives));
		break;
	}
}

dae::LivesDisplayObserver::LivesDisplayObserver(GameObject* textToUpdate)
{
	m_livesTextComp = textToUpdate->getComponent<TextComponent>();
}

dae::ScoreDisplayObserver::ScoreDisplayObserver(GameObject* textToUpdate)
{
	m_scoreTextComp = textToUpdate->getComponent<TextComponent>();
}

void dae::ScoreDisplayObserver::Notify(GameEvent event, GameObject* actor)
{
	switch (event)
	{
	case GameEvent::PlayerScored:
		auto score = actor->getComponent<PacManCharacter>()->GetScore();
		m_scoreTextComp->SetText("Score: " + std::to_string(score));
		break;
	}
}
