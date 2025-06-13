#pragma once
#include "RenderComponent.h"
#include "GridComponent.h"
#include "PacManCharacters.h"

namespace dae
{ 
    enum class GhostType
    {
        Blinky,
        Pinky,
        Inky,
        Sue,
        Player
    };

    class GhostComponent; // Forward declaration
    class GhostMovingState;
    class GhostIdleState;
    class GhostFleeState;
    class GhostReturnState;

	class GhostState
    {
    public:
        GhostState() = default;

        virtual ~GhostState() = default;

        virtual void Update(GhostComponent& ghost, float deltaTime) = 0;
        virtual void OnEnter(GhostComponent& ghost) = 0;
        virtual void OnExit(GhostComponent& ghost) = 0;

        static std::unique_ptr<GhostMovingState> m_movingState;
        static std::unique_ptr<GhostIdleState> m_idleState;
		static std::unique_ptr<GhostFleeState> m_fleeState;
        static std::unique_ptr<GhostReturnState> m_returnState;

        static std::unique_ptr<GhostMovingState> m_pinkyMovingState;
        static std::unique_ptr<GhostIdleState> m_pinkyIdleState;
		static std::unique_ptr<GhostFleeState> m_pinkyFleeState;
        static std::unique_ptr<GhostReturnState> m_pinkyReturnState;

        static std::unique_ptr<GhostMovingState> m_inkyMovingState;
        static std::unique_ptr<GhostIdleState> m_inkyIdleState;
		static std::unique_ptr<GhostFleeState> m_inkyFleeState;
        static std::unique_ptr<GhostReturnState> m_inkyReturnState;

        static std::unique_ptr<GhostMovingState> m_sueMovingState;
        static std::unique_ptr<GhostIdleState> m_sueIdleState;
		static std::unique_ptr<GhostFleeState> m_sueFleeState;
        static std::unique_ptr<GhostReturnState> m_sueReturnState;
    protected:
    };

    class GhostMovingState : public GhostState
    {
    public:
        explicit GhostMovingState() {};
        void Update(GhostComponent& ghost, float deltaTime) override;
        void OnEnter(GhostComponent& ghost) override;
        void OnExit(GhostComponent& ghost) override;

        static glm::vec3 FindNextMoveDirection(int startX, int startY, int targetX, int targetY, GridComponent* pGrid);

        void FindNearestWalkableCell(int startX, int startY, GridComponent* pGrid, int& outX, int& outY);

        void SetShouldReset(bool shouldReset)
        {
            m_shouldReset = shouldReset;
        }

    protected:
        bool m_shouldReset = true;
    };

    class GhostIdleState : public GhostState
    {
    public:
        explicit GhostIdleState() = default;
        void Update(GhostComponent& ghost, float dt) override;
        void OnEnter(GhostComponent& ghost) override ;
        void OnExit(GhostComponent&) override ;
    private:
        float m_startDelay;
    };

    class GhostFleeState : public GhostState
    {
    public:
        explicit GhostFleeState() = default;
		void Update(GhostComponent& ghost, float deltaTime) override;
        void OnEnter(GhostComponent& ghost) override;
		void OnExit(GhostComponent& ghost) override;

        void SetTimer(float timer)
        {
            m_fleeTimer = timer;
		}
    private:
		float m_fleeTimer = 0.0f;
    };

    class GhostReturnState : public GhostState
    {
    public:
        explicit GhostReturnState() = default;
        void Update(GhostComponent& ghost, float deltaTime) override;
        void OnEnter(GhostComponent& ghost) override;
        void OnExit(GhostComponent& ghost) override;
    };

    class GhostComponent :
    public RenderComponent
    {
    public:
        GhostComponent(GameObject& gameObject, GridComponent* pGridComponent, GhostType type ,float startDelay);

        void Update(float deltaTime) override;

        utils::Vector2f GetMiddlePosition() const
        {
            auto world = GetOwner()->GetWorldPosition();
            auto size = m_DestRect;
            utils::Vector2f vec{ world.x + size.width / 2, world.y + size.height / 2 };
            return vec;
        }

        void SetMiddlePosition(float x, float y)
        {
            GetOwner()->SetWorldPosition(x - m_DestRect.width / 2, y - m_DestRect.height / 2);
        }

        GhostState* GetGhostManState() const
        {
            return m_ghostState;
        }

        void SetGhostState(GhostState* state)
        {
            if (m_ghostState)
            {
                m_ghostState->OnExit(*this);
            }
            m_ghostState = state;
            if (m_ghostState)
            {
                m_ghostState->OnEnter(*this);
            }
		}

        GhostState* GetGhostState() const
        {
            return m_ghostState;
        }

        GridComponent* GetGridComponent() const
        {
            return m_pGridComponent;
		}

        void AddEnemy(dae::PacManCharacter* enemy)
        {
            m_enemies.push_back(enemy);
		}

        std::vector<dae::PacManCharacter*> GetEnemies() const
        {
            return m_enemies;
        }

        void SetMovementDirection(const glm::vec2& dir)
        {
            m_movementDir = dir;
		}

        float GetOriginalStartDelay()
        {
			return m_originalStartDelay;
        }

        GhostType GetGhostType() const
        {
            return m_ghostType;
		}

		void CheckEnemyCollision();

        void ResetGhosts();


        void UpdateInkySprite(float deltaTime);
        void UpdatePinkySprite(float deltaTime);
        void UpdateBlinkySprite(float deltaTime);
        void UpdateSueSprite(float deltaTime);

        void UpdateFleeSprite(float deltaTime);

		void UpdateReturnSprite(float deltaTime);

        void SetSpeed(float speed = 100)
        { 
            m_Speed = speed;
		}

        //FOR PLAYER ONLY
        bool CanMoveUp() const;
        bool CanMoveDown() const;
        bool CanMoveLeft() const;
        bool CanMoveRight() const;

        void MoveLeft(float deltaTime);
        void MoveRight(float deltaTime);
        void MoveUp(float deltaTime);
        void MoveDown(float deltaTime);
        //FOR PLAYER ONLY
    private:
		

        GhostState* m_ghostState = nullptr;
        float m_Speed = 100.0f; // Speed of the ghost
        float m_spriteTimer;
        float m_spriteInterval;
        int m_spriteIndex;

        GridComponent* m_pGridComponent;

		const float m_originalStartDelay;

        std::vector<dae::PacManCharacter*> m_enemies;

        glm::vec2 m_movementDir;

        GhostType m_ghostType{ GhostType::Blinky };
    };
}

