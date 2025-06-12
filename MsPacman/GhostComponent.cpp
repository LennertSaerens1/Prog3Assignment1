#include "GhostComponent.h"
#include <queue>

namespace dae
{

    std::unique_ptr<GhostMovingState> GhostState::m_movingState = nullptr;
    std::unique_ptr<GhostIdleState> GhostState::m_idleState = nullptr;
    std::unique_ptr<GhostFleeState> GhostState::m_fleeState = nullptr;
    std::unique_ptr<GhostReturnState> GhostState::m_returnState = nullptr;

    std::unique_ptr<GhostMovingState> GhostState::m_pinkyMovingState = nullptr;
    std::unique_ptr<GhostIdleState> GhostState::m_pinkyIdleState = nullptr;
    std::unique_ptr<GhostFleeState> GhostState::m_pinkyFleeState = nullptr;
    std::unique_ptr<GhostReturnState> GhostState::m_pinkyReturnState = nullptr;

    std::unique_ptr<GhostMovingState> GhostState::m_inkyMovingState = nullptr;
    std::unique_ptr<GhostIdleState> GhostState::m_inkyIdleState = nullptr;
    std::unique_ptr<GhostFleeState> GhostState::m_inkyFleeState = nullptr;
    std::unique_ptr<GhostReturnState> GhostState::m_inkyReturnState = nullptr;

    std::unique_ptr<GhostMovingState> GhostState::m_sueMovingState = nullptr;
    std::unique_ptr<GhostIdleState> GhostState::m_sueIdleState = nullptr;
    std::unique_ptr<GhostFleeState> GhostState::m_sueFleeState = nullptr;
    std::unique_ptr<GhostReturnState> GhostState::m_sueReturnState = nullptr;

    void GhostMovingState::Update(GhostComponent& ghost, float)
    {
        // Get grid component and players
        auto* pGrid = ghost.GetGridComponent();
        if (!pGrid) return;

        const auto& players = ghost.GetEnemies();
        if (players.empty()) return;

        // Get ghost's current position
        utils::Vector2f ghostPos = ghost.GetMiddlePosition();

        // Find nearest player
        PacManCharacter* nearestPlayer = nullptr;
        float shortestDistance = std::numeric_limits<float>::max();

        for (auto* player : players) {
            if (!player) continue;

            // Calculate Manhattan distance to this player
            utils::Vector2f playerPos = player->GetMiddlePosition();
            float distance = std::abs(playerPos.x - ghostPos.x) + std::abs(playerPos.y - ghostPos.y);

            if (distance < shortestDistance) {
                nearestPlayer = player;
                shortestDistance = distance;
            }
        }

        if (!nearestPlayer) return;

        // Determine direction to move toward the nearest player
        utils::Vector2f targetPos = nearestPlayer->GetMiddlePosition();


        // Get grid coordinates
		int ghostCellX = pGrid->GetCellXFromWorld(ghostPos);
		int ghostCellY = pGrid->GetCellYFromWorld(ghostPos);
		int targetCellX = pGrid->GetCellXFromWorld(targetPos);
		int targetCellY = pGrid->GetCellYFromWorld(targetPos);



        // Find the best direction to move using BFS
        glm::vec3 moveDir = FindNextMoveDirection(ghostCellX, ghostCellY, targetCellX, targetCellY, pGrid);

        // Move the ghost
		ghost.SetMovementDirection(moveDir);
    }

    void GhostMovingState::OnEnter(GhostComponent& ghost)
    { 
		auto inkyWorld = ghost.GetGridComponent()->GetWorldCoordinatesMiddle(13, 11);

        ghost.SetMiddlePosition(inkyWorld.x, inkyWorld.y);
    }
    void GhostMovingState::OnExit(GhostComponent& )
    { }

    glm::vec3 GhostMovingState::FindNextMoveDirection(int startX, int startY, int targetX, int targetY, GridComponent* pGrid)
    {
        // Directions: up, right, down, left
        const int dx[] = { 0, 1, 0, -1 };
        const int dy[] = { -1, 0, 1, 0 };

        // Grid dimensions
        int width = pGrid->GetWidth();
        int height = pGrid->GetHeight();

        // BFS queue - stores {x, y, first_move_direction}
        std::queue<std::tuple<int, int, int>> queue;
        std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

        // Add starting position with all possible directions
        for (int dir = 0; dir < 4; dir++) {
            int nextX = startX + dx[dir];
            int nextY = startY + dy[dir];

            if (nextX >= 0 && nextX < width && nextY >= 0 && nextY < height && pGrid->IsWalkable(nextX, nextY)) {
                queue.push({ nextX, nextY, dir });
                visited[nextY][nextX] = true;
            }
        }

        // BFS to find shortest path
        while (!queue.empty()) {
            auto [x, y, firstMove] = queue.front();
            queue.pop();

            // If we reached the target
            if (x == targetX && y == targetY) {
                // Return the direction of the first move
                switch (firstMove) {
                case 0: return glm::vec3(0, -1, 0); // Up
                case 1: return glm::vec3(1, 0, 0);  // Right
                case 2: return glm::vec3(0, 1, 0);  // Down
                case 3: return glm::vec3(-1, 0, 0); // Left
                }
            }

            // Try all four directions
            for (int dir = 0; dir < 4; dir++) {
                int nextX = x + dx[dir];
                int nextY = y + dy[dir];

                if (nextX >= 0 && nextX < width && nextY >= 0 && nextY < height &&
                    pGrid->IsWalkable(nextX, nextY) && !visited[nextY][nextX]) {
                    queue.push({ nextX, nextY, firstMove });
                    visited[nextY][nextX] = true;
                }
            }
        }

        std::cout << "No path found from (" << startX << ", " << startY << ") to ("<< targetX << ", " << targetY << ")\n";
		return glm::vec3(0, 0, 0); // No valid move found
    }

    void GhostIdleState::Update(GhostComponent& ghost, float deltaTime)
    {
        m_startDelay -= deltaTime;
        if (m_startDelay <= 0.0f)
        {
            switch (ghost.GetGhostType())
            {
            case dae::GhostType::Blinky:
                ghost.SetGhostState(GhostState::m_movingState.get());
                break;
                case dae::GhostType::Pinky:
                    ghost.SetGhostState(GhostState::m_pinkyMovingState.get());
                    break;
				case dae::GhostType::Inky:
                    ghost.SetGhostState(GhostState::m_inkyMovingState.get());
                    break;
				case dae::GhostType::Sue:
                    ghost.SetGhostState(GhostState::m_sueMovingState.get());
                    break;
            }
            return;
        }
    };

    void GhostIdleState::OnEnter(GhostComponent& ghost)
    {
        m_startDelay = ghost.GetOriginalStartDelay();
    }

    void GhostIdleState::OnExit(GhostComponent& )
    {

    }

    void GhostFleeState::Update(GhostComponent& , float )
    {

	}

    void GhostFleeState::OnEnter(GhostComponent& )
    {
    }
    void GhostFleeState::OnExit(GhostComponent& )
    {
    }

    void GhostReturnState::Update(GhostComponent& , float )
    {

    }
    void GhostReturnState::OnEnter(GhostComponent& )
    {
    }
    void GhostReturnState::OnExit(GhostComponent& )
    {
    }

        

    dae::GhostComponent::GhostComponent(GameObject& gameObject, GridComponent* pGridComponent, GhostType type, float startDelay)
        : RenderComponent("SpriteSheet.png", gameObject)
        , m_pGridComponent(pGridComponent)
        , m_spriteTimer(0.0f)
        , m_spriteInterval(0.5f) // Adjust sprite change interval as needed
        , m_spriteIndex(0)
        , m_originalStartDelay(startDelay)
        , m_ghostType(type)
    {
       

        switch (type)
        {
        case dae::GhostType::Blinky:
            GhostState::m_movingState = std::make_unique<GhostMovingState>();
            GhostState::m_idleState = std::make_unique<GhostIdleState>();
            GhostState::m_fleeState = std::make_unique<GhostFleeState>();
            GhostState::m_returnState = std::make_unique<GhostReturnState>();

            SetGhostState(GhostState::m_idleState.get());
            break;
        case dae::GhostType::Pinky:
            GhostState::m_pinkyMovingState = std::make_unique<GhostMovingState>();
            GhostState::m_pinkyIdleState = std::make_unique<GhostIdleState>();
            GhostState::m_pinkyFleeState = std::make_unique<GhostFleeState>();
            GhostState::m_pinkyReturnState = std::make_unique<GhostReturnState>();

            SetGhostState(GhostState::m_pinkyIdleState.get());
            break;
        case dae::GhostType::Inky:
            GhostState::m_inkyMovingState = std::make_unique<GhostMovingState>();
            GhostState::m_inkyIdleState = std::make_unique<GhostIdleState>();
            GhostState::m_inkyFleeState = std::make_unique<GhostFleeState>();
            GhostState::m_inkyReturnState = std::make_unique<GhostReturnState>();

            SetGhostState(GhostState::m_inkyIdleState.get());
            break;
        case dae::GhostType::Sue:
            GhostState::m_sueMovingState = std::make_unique<GhostMovingState>();
            GhostState::m_sueIdleState = std::make_unique<GhostIdleState>();
            GhostState::m_sueFleeState = std::make_unique<GhostFleeState>();
            GhostState::m_sueReturnState = std::make_unique<GhostReturnState>();

            SetGhostState(GhostState::m_sueIdleState.get());
            break;
        }


        SetSrcRect(utils::Rect{ 457, 65, 14, 14 });
        SetDestRect(utils::Rect{ 0, 0, 14*3, 14*3 });
    }

    void GhostComponent::Update(float deltaTime)
    {
		RenderComponent::Update(deltaTime);

        m_ghostState->Update(*this, deltaTime);

		auto movement = m_movementDir * m_Speed * deltaTime;

		GetOwner()->AddWorldOffset(glm::vec3{movement.x, movement.y, 0});

        switch (m_ghostType)
        {
        case dae::GhostType::Blinky:
            UpdateBlinkySprite(deltaTime);
            break;
        case dae::GhostType::Pinky:
            UpdatePinkySprite(deltaTime);
            break;
        case dae::GhostType::Inky:
            UpdateInkySprite(deltaTime);
            break;
        case dae::GhostType::Sue:
            UpdateSueSprite(deltaTime);
            break;
        default:
            break;
        }
	}
    void GhostComponent::UpdateInkySprite(float deltaTime)
    {
        m_spriteTimer += deltaTime;

        if (m_spriteTimer > m_spriteInterval)
        {
            ++m_spriteIndex;
            m_spriteTimer = 0;

            if (m_spriteIndex >= 2)
                m_spriteIndex = 0;
        }
        if (m_movementDir == glm::vec2{ 1,0 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 457, 97, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 473, 97, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ -1,0 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 489, 97, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 505, 97, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ 0,-1 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 520, 97, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 536, 97, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ 0,1 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 553, 97, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 569, 97, 14, 14 });
                break;
            default:
                break;
            }
        }
        else
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 457, 97, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 473, 97, 14, 14 });
                break;
            default:
                break;
            }
        }
    }
    void GhostComponent::UpdatePinkySprite(float deltaTime)
    {
        m_spriteTimer += deltaTime;
        if (m_spriteTimer > m_spriteInterval)
        {
            ++m_spriteIndex;
            m_spriteTimer = 0;

            if (m_spriteIndex >= 2)
                m_spriteIndex = 0;
        }
        if (m_movementDir == glm::vec2{ 1,0 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 457, 81, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 473, 81, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ -1,0 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 489, 81, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 505, 81, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ 0,-1 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 520, 81, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 536, 81, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ 0,1 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 553, 81, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 569, 81, 14, 14 });
                break;
            default:
                break;
            }
        }
        else
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 457, 81, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 473, 81, 14, 14 });
                break;
            default:
                break;
            }
        }
    }
    void GhostComponent::UpdateBlinkySprite(float deltaTime)
    {
        m_spriteTimer += deltaTime;
        if (m_spriteTimer > m_spriteInterval)
        {
            ++m_spriteIndex;
            m_spriteTimer = 0;

            if (m_spriteIndex >= 2)
                m_spriteIndex = 0;
        }
        if (m_movementDir == glm::vec2{ 1,0 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 457, 65, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 473, 65, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ -1,0 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 489, 65, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 505, 65, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ 0,-1 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 520, 65, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 536, 65, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ 0,1 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 553, 65, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 569, 65, 14, 14 });
                break;
            default:
                break;
            }
        }
        else
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 457, 65, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 473, 65, 14, 14 });
                break;
            default:
                break;
            }
        }
    }
    void GhostComponent::UpdateSueSprite(float deltaTime)
    {
        m_spriteTimer += deltaTime;
        if (m_spriteTimer > m_spriteInterval)
        {
            ++m_spriteIndex;
            m_spriteTimer = 0;
            if (m_spriteIndex >= 2)
                m_spriteIndex = 0;
        }
        if (m_movementDir == glm::vec2{ 1,0 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 457, 113, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 473, 113, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ -1,0 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 489, 113, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 505, 113, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ 0,-1 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 520, 113, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 536, 113, 14, 14 });
                break;
            default:
                break;
            }
        }
        else if (m_movementDir == glm::vec2{ 0,1 })
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 553, 113, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 569, 113, 14, 14 });
                break;
            default:
                break;
            }
        }
        else
        {
            switch (m_spriteIndex)
            {
            case 0:
                SetSrcRect(utils::Rect{ 457, 113, 14, 14 });
                break;
            case 1:
                SetSrcRect(utils::Rect{ 473, 113, 14, 14 });
                break;
            default:
                break;
            }
        }
    }
}