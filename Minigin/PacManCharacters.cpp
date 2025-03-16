#include "PacManCharacters.h"

dae::PacManCharacter::PacManCharacter(GameObject& gameObject, bool isMale)
	: RenderComponent("PacMan.png", gameObject)
{
	m_lives = 3;
	m_score = 0;
	if (!isMale)
	{
		SetTexture("MsPacMan.png");
	}
}

void dae::PacManCharacter::DecreaseLives()
{
	m_lives--;
	GetOwner()->NotifyObservers(GameEvent::PlayerDied);
}

void dae::PacManCharacter::AddScore(int score)
{
	m_score += score;
	GetOwner()->NotifyObservers(GameEvent::PlayerScored);
}

int dae::PacManCharacter::GetLives() const
{
	return m_lives;
}

int dae::PacManCharacter::GetScore() const
{
	return m_score;
}