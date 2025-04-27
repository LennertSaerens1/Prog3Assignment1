#include "PacManCharacters.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"


dae::PacManCharacter::PacManCharacter(GameObject& gameObject, bool isMale)
	: RenderComponent("PacMan.png", gameObject)
{
	m_lives = 3;
	m_score = 0;
	if (!isMale)
	{
		SetTexture("MsPacMan.png");
	}

	auto& soundSystem = dae::ServiceLocator::get_sound_system();
	m_loopingId = soundSystem.loadSound("Sounds/credit.wav");
	soundSystem.play(m_loopingId, 0.06f, true);

}

void dae::PacManCharacter::DecreaseLives()
{
	m_lives--;
	auto& soundSystem = dae::ServiceLocator::get_sound_system();
	auto id = soundSystem.loadSound("Sounds/ms_death.wav");
	soundSystem.play(id, 1.0f);
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