#pragma once
#include "RenderComponent.h"
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
    };
}

