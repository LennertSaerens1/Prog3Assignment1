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
        ~PacManCharacter();
        void DecreaseLives();
		void AddScore(int score);

        int GetLives() const;
		int GetScore() const;

		utils::Vector2f GetMiddlePosition() const
		{
			auto world = GetOwner()->GetWorldPosition();
			auto size = m_DestRect;
			Vector2f vec { world.x + size.width / 2, world.y + size.height / 2 };
			return vec;
		}

		void SetMiddlePosition(float x, float y)
		{
			GetOwner()->SetWorldPosition(x - m_DestRect.width / 2, y - m_DestRect.height / 2);
		}
    private:
        int m_lives;
		int m_score;
        dae::SoundId m_loopingId;
    };
}

