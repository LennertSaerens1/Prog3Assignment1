#include "Displays.h"
#include "PacManCharacters.h"
#include "GameObject.h"

void dae::LivesDisplayObserver::Notify(GameEvent event, GameObject* actor)
{
	switch (event)
	{
	case GameEvent::PlayerDied:
		auto lives = actor->getComponent<PacManCharacter>()->GetLives();
		switch (lives)
		{
		case 5:
			m_livesRenderComp->SetSrcRect(utils::Rect{ 696, 96, 80, 16 });
			break;
		case 4:
			m_livesRenderComp->SetSrcRect(utils::Rect{ 696, 112, 80, 16 });
			break;
		case 3:
			m_livesRenderComp->SetSrcRect(utils::Rect{ 696, 128, 80, 16 });
			break;
		case 2:
			m_livesRenderComp->SetSrcRect(utils::Rect{ 696, 144, 80, 16 });
			break;
		case 1:
			m_livesRenderComp->SetSrcRect(utils::Rect{ 696, 160, 80, 16 });
			break;
		case 0:
			m_livesRenderComp->SetSrcRect(utils::Rect{ 560, 60, 80, 16 });
			break;
		default:
			break;
		}
		break;
	}
}

dae::LevelUpObserver::LevelUpObserver(GameObject* pacMan)
{
	m_pacMan = pacMan->getComponent<PacManCharacter>();
}

dae::LivesDisplayObserver::LivesDisplayObserver(GameObject* textToUpdate)
{
	m_livesRenderComp = textToUpdate->getComponent<RenderComponent>();
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
		m_scoreTextComp->SetText(std::to_string(score));
		break;
	}
}

void dae::LevelUpObserver::Notify(GameEvent event, GameObject* )
{
	switch (event)
	{
	case GameEvent::nextLevel:
		m_pacMan->LevelUp();
		break;
	case GameEvent::PlayerDied:
		//m_pacMan->ResetLevel();
		break;
	default:
		break;
	}
}
