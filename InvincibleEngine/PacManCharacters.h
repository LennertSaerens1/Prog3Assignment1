#pragma once
#include "RenderComponent.h"
#include "SoundSystem.h"
namespace dae
{
    class PacManCharacter :
        public RenderComponent
    {
	public:
		PacManCharacter(GameObject& gameObject, bool isMale = true);

        void DecreaseLives();
		void AddScore(int score);

        int GetLives() const;
		int GetScore() const;
    private:
        int m_lives;
		int m_score;
        dae::sound_id m_loopingId;
    };
}

