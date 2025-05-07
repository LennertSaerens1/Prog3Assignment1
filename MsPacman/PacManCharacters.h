#pragma once
#include "RenderComponent.h"
#include "SoundSystem.h"

namespace dae
{
    class PacManCharacter; // Forward declaration
    class DownState;
	class LeftState;
	class RightState;
	class UpState;

    class PacManState
    {
    public:
		PacManState(PacManCharacter& player) : m_pacManCharacter(&player) {}


        virtual ~PacManState() = default;

        virtual void Update(PacManCharacter& player, float deltaTime) = 0;


        virtual void OnEnter(PacManCharacter& player) =0;
		virtual void OnExit(PacManCharacter& player) = 0;

        static std::unique_ptr<DownState> m_downState;
        static std::unique_ptr<UpState> m_upState;
        static std::unique_ptr<LeftState> m_leftState;
        static std::unique_ptr<RightState> m_rightState;
    protected:

		PacManCharacter* m_pacManCharacter;
    };

    class DownState : public PacManState
    {
    public:
        explicit DownState(PacManCharacter& player) : PacManState(player) {}
        void Update(PacManCharacter& player, float deltaTime) override;
        void OnEnter(PacManCharacter& player) override;
        void OnExit(PacManCharacter& player) override;
    };

    class LeftState : public PacManState
    {
    public:
        explicit LeftState(PacManCharacter& player) : PacManState(player) {}
        void Update(PacManCharacter& player, float deltaTime) override;
        void OnEnter(PacManCharacter& player) override;
        void OnExit(PacManCharacter& player) override;
    };

    class RightState : public PacManState
    {
    public:
        explicit RightState(PacManCharacter& player) : PacManState(player) {}
        void Update(PacManCharacter& player, float deltaTime) override;
        void OnEnter(PacManCharacter& player) override;
        void OnExit(PacManCharacter& player) override;
    };

    class UpState : public PacManState
    {
    public:
        explicit UpState(PacManCharacter& player) : PacManState(player) {}
        void Update(PacManCharacter& player, float deltaTime) override;
        void OnEnter(PacManCharacter& player) override;
        void OnExit(PacManCharacter& player) override;
    };


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

		PacManState* GetPacManState() const
		{
			return m_pacManState;
		}

		void MoveLeft(float deltaTime);
		void MoveRight(float deltaTime);
		void MoveUp(float deltaTime);
		void MoveDown(float deltaTime);

        void SetState(PacManState* newState);
        virtual void Update(float deltaTime) override;
    private:
        int m_lives;
		int m_score;
        dae::SoundId m_loopingId;
		float m_movementSpeed;

        PacManState* m_pacManState;
		float m_spriteTimer;
		float m_spriteInterval;
		int m_spriteIndex;
    };
}

