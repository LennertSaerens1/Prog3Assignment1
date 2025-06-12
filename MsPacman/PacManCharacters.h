#pragma once
#include "RenderComponent.h"
#include "SoundSystem.h"
#include "GridComponent.h"
#include "Utils.h"

namespace dae
{
    class PacManCharacter; // Forward declaration
    class MovingState;
    class IdleState;

    class PacManState
    {
    public:
		PacManState(PacManCharacter& player) : m_pacManCharacter(&player) {}

        virtual ~PacManState() = default;

        virtual void Update(PacManCharacter& player, float deltaTime) = 0;

        virtual void OnEnter(PacManCharacter& player) =0;
		virtual void OnExit(PacManCharacter& player) = 0;

        static std::unique_ptr<MovingState> m_movingState;
		static std::unique_ptr<IdleState> m_idleState;

        static std::unique_ptr<MovingState> m_mrMovingState;
		static std::unique_ptr<IdleState> m_mrIdleState;


    protected:

		PacManCharacter* m_pacManCharacter;
    };

    class MovingState : public PacManState
    {
    public:
        explicit MovingState(PacManCharacter& player) : PacManState(player) {}
        void Update(PacManCharacter& player, float deltaTime) override;
        void OnEnter(PacManCharacter& player) override;
        void OnExit(PacManCharacter& player) override;
    };

    class IdleState : public PacManState
    {
    public:
        explicit IdleState(PacManCharacter& player) : PacManState(player) {}
        void Update(PacManCharacter& , float ) override {};
        void OnEnter(PacManCharacter& ) override {};
        void OnExit(PacManCharacter& ) override {};
    };


    class PacManCharacter :
        public RenderComponent
    {
	public:
		PacManCharacter(GameObject& gameObject, GridComponent* pGridComponent, bool isMale = false);
        ~PacManCharacter();
        void DecreaseLives();
		void AddScore(int score);

        int GetLives() const;
		int GetScore() const;

		utils::Vector2f GetMiddlePosition() const
		{
			auto world = GetOwner()->GetWorldPosition();
			auto size = m_DestRect;
			utils::Vector2f vec { world.x + size.width / 2, world.y + size.height / 2 };
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

        bool CanMoveUp() const;
		bool CanMoveDown() const;
		bool CanMoveLeft() const;
		bool CanMoveRight() const;

		void MoveLeft(float deltaTime);
		void MoveRight(float deltaTime);
		void MoveUp(float deltaTime);
		void MoveDown(float deltaTime);

		void PickUp(float deltaTime);

        void SetState(PacManState* newState);
        void SetMovementDirection(glm::vec3 movementDir);
        virtual void Update(float deltaTime) override;

        bool GetIsMale() { return m_isMale; };

        void AddOtherPlayer(PacManCharacter* otherCharacter) ;

		glm::vec3 GetMovementDirection() const
		{
			return m_movementDirection;
		}

		void LevelUp();
    private:
        int m_lives;
		int m_score;
        int m_isMale;
		float m_movementSpeed;

        glm::vec3 m_movementDirection;

        PacManState* m_pacManState;
		float m_spriteTimer;
		float m_spriteInterval;
		int m_spriteIndex;

		GridComponent* m_pGridComponent;

        PacManCharacter* m_otherPlayer;
    };
}

