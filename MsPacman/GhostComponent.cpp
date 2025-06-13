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

    void GhostMovingState::Update(GhostComponent& ghost, float deltaTime)
    {
        switch (ghost.GetGhostType())
        {
        case dae::GhostType::Blinky:
            ghost.UpdateBlinkySprite(deltaTime);
            break;
        case dae::GhostType::Pinky:
            ghost.UpdatePinkySprite(deltaTime);
            break;
        case dae::GhostType::Inky:
            ghost.UpdateInkySprite(deltaTime);
            break;
        case dae::GhostType::Sue:
            ghost.UpdateSueSprite(deltaTime);
            break;
		case dae::GhostType::Player:
			ghost.UpdateBlinkySprite(deltaTime);
			ghost.SetSpeed(100);
            return;
			break;
        default:
            break;
        }

        // Get grid component and players
        auto* pGrid = ghost.GetGridComponent();
        if (!pGrid) return;

        const auto& players = ghost.GetEnemies();
        if (players.empty()) return;

        // Get ghost's current position
        utils::Vector2f ghostPos = ghost.GetMiddlePosition();
        int ghostCellX = pGrid->GetCellXFromWorld(ghostPos);
        int ghostCellY = pGrid->GetCellYFromWorld(ghostPos);

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

        // Get player position and determine target based on ghost type
        utils::Vector2f playerPos = nearestPlayer->GetMiddlePosition();
        int playerCellX = pGrid->GetCellXFromWorld(playerPos);
        int playerCellY = pGrid->GetCellYFromWorld(playerPos);

        // Determine target position based on ghost type
        int targetCellX = playerCellX;
        int targetCellY = playerCellY;

        // Get player's movement direction
        glm::vec2 playerDir = nearestPlayer->GetMovementDirection();

        // Grid dimensions for bounds checking
        int width = pGrid->GetWidth();
        int height = pGrid->GetHeight();

        GhostComponent* blinky = nullptr;
        float pinkyDistanceToPlayer{};

        switch (ghost.GetGhostType())
        {
        case dae::GhostType::Blinky:
            // Blinky directly targets Pac-Man's position
            break;

        case dae::GhostType::Pinky:
            // Calculate tile distance to player to determine behavior
            pinkyDistanceToPlayer = std::sqrtf(
                std::powf(static_cast<float>(playerCellX) - ghostCellX, 2) +
                std::powf(static_cast<float>(playerCellY) - ghostCellY, 2));

            // If Pinky is very close to the player (within 3 tiles), chase directly
            if (pinkyDistanceToPlayer < 5.0f) {
                // Directly target player's position
                targetCellX = playerCellX;
                targetCellY = playerCellY;
            }
            else {
                // Normal behavior: target 4 tiles ahead of Pac-Man
                if (std::abs(playerDir.x) > std::abs(playerDir.y)) {
                    // Moving horizontally
                    targetCellX = playerCellX + (playerDir.x > 0 ? 4 : -4);
                }
                else {
                    // Moving vertically
                    targetCellY = playerCellY + (playerDir.y > 0 ? 4 : -4);
                }
            }
            break;
        case dae::GhostType::Inky:
            // Inky uses a more complex targeting
            // First, get the position 2 tiles ahead of Pac-Man
            int inkyTargetX, inkyTargetY;
            if (std::abs(playerDir.x) > std::abs(playerDir.y)) {
                inkyTargetX = playerCellX + (playerDir.x > 0 ? 2 : -2);
                inkyTargetY = playerCellY;
            }
            else {
                inkyTargetX = playerCellX;
                inkyTargetY = playerCellY + (playerDir.y > 0 ? 2 : -2);
            }

            // Clamp intermediate target to grid boundaries
            inkyTargetX = std::max(0, std::min(inkyTargetX, width - 1));
            inkyTargetY = std::max(0, std::min(inkyTargetY, height - 1));

            // Now find Blinky's position
            for (auto* ghostComponent : pGrid->GetGhosts()) {
                if (ghostComponent && ghostComponent->GetGhostType() == dae::GhostType::Blinky) {
                    blinky = ghostComponent;
                    break;
                }
            }

            if (blinky) {
                utils::Vector2f blinkyPos = blinky->GetMiddlePosition();
                int blinkyCellX = pGrid->GetCellXFromWorld(blinkyPos);
                int blinkyCellY = pGrid->GetCellYFromWorld(blinkyPos);

                // Target is twice the vector from Blinky to the point 2 tiles ahead of Pac-Man
                targetCellX = inkyTargetX + (inkyTargetX - blinkyCellX);
                targetCellY = inkyTargetY + (inkyTargetY - blinkyCellY);
            }
            break;

        case dae::GhostType::Sue:
            // Sue (Clyde) targets Pac-Man directly if far away, but retreats to a corner when close
            // Calculate Euclidean distance (in tiles) to Pac-Man
            float tileDistance = std::sqrtf(
                std::powf(static_cast<float>(playerCellX) - ghostCellX, 2) +
                std::powf(static_cast<float>(playerCellY) - ghostCellY, 2));

            // Sue has random behavior sometimes
            static float randomTimer = 0.0f;
            randomTimer -= deltaTime;

            if (randomTimer <= 0.0f) {
                // Reset timer and maybe change target
                randomTimer = 5.0f + static_cast<float>(rand() % 5); // 5-10 seconds

                if (rand() % 2 == 0) { // 50% chance to pick a random spot
                    targetCellX = rand() % width;
                    targetCellY = rand() % height;
                    break; // Skip the normal Sue logic
                }
            }

            if (tileDistance < 8) {
                // If close to Pac-Man, retreat to lower-left corner
                targetCellX = 1;
                targetCellY = pGrid->GetHeight() - 2;
            }
            break;
        }

        // Clamp target position to grid boundaries
        targetCellX = std::max(0, std::min(targetCellX, width - 1));
        targetCellY = std::max(0, std::min(targetCellY, height - 1));

        // Check if the target position is walkable, if not find nearest walkable cell
        if (!pGrid->IsWalkable(targetCellX, targetCellY)) {
            // Find the nearest walkable cell to the target
            FindNearestWalkableCell(targetCellX, targetCellY, pGrid, targetCellX, targetCellY);
        }

        // Find the best direction to move using BFS
        glm::vec3 moveDir = FindNextMoveDirection(ghostCellX, ghostCellY, targetCellX, targetCellY, pGrid);

        ghost.SetSpeed();

        // Move the ghost
        ghost.SetMovementDirection(moveDir);
    }

    // Helper method to find the nearest walkable cell to a target position
    void GhostMovingState::FindNearestWalkableCell(int startX, int startY, GridComponent* pGrid,
        int& outX, int& outY)
    {
        // Grid dimensions
        int width = pGrid->GetWidth();
        int height = pGrid->GetHeight();

        // Simple BFS to find the nearest walkable cell
        std::queue<std::pair<int, int>> queue;
        std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

        // Start at the given position
        queue.push({ startX, startY });
        visited[startY][startX] = true;

        // Directions: up, right, down, left
        const int dx[] = { 0, 1, 0, -1 };
        const int dy[] = { -1, 0, 1, 0 };

        while (!queue.empty()) {
            auto [x, y] = queue.front();
            queue.pop();

            // If this cell is walkable, use it
            if (pGrid->IsWalkable(x, y)) {
                outX = x;
                outY = y;
                return;
            }

            // Try all four directions
            for (int dir = 0; dir < 4; dir++) {
                int nextX = x + dx[dir];
                int nextY = y + dy[dir];

                if (nextX >= 0 && nextX < width && nextY >= 0 && nextY < height &&
                    !visited[nextY][nextX]) {
                    queue.push({ nextX, nextY });
                    visited[nextY][nextX] = true;
                }
            }
        }

        // If no walkable cell found, fallback to original position
        outX = startX;
        outY = startY;
    }

    void GhostMovingState::OnEnter(GhostComponent& ghost)
    { 
        if (!m_shouldReset) return;
		m_shouldReset = false;

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
        switch (ghost.GetGhostType())
        {
        case dae::GhostType::Blinky:
            ghost.UpdateBlinkySprite(deltaTime);
            break;
        case dae::GhostType::Pinky:
            ghost.UpdatePinkySprite(deltaTime);
            break;
        case dae::GhostType::Inky:
            ghost.UpdateInkySprite(deltaTime);
            break;
        case dae::GhostType::Sue:
            ghost.UpdateSueSprite(deltaTime);
            break;
		case dae::GhostType::Player:
			ghost.UpdateBlinkySprite(deltaTime);
            break;
        default:
            break;
        }

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
				case dae::GhostType::Player:
                    ghost.SetGhostState(GhostState::m_movingState.get());
					break;
            }
            return;
        }
    };

    void GhostIdleState::OnEnter(GhostComponent& ghost)
    {
		ghost.SetMovementDirection(glm::vec3(0, 0, 0)); // Stop movement

        m_startDelay = ghost.GetOriginalStartDelay();
    }

    void GhostIdleState::OnExit(GhostComponent& )
    {

    }

    void GhostFleeState::Update(GhostComponent& ghost, float deltaTime)
    {
		ghost.UpdateFleeSprite(deltaTime);

        // Update flee timer
        m_fleeTimer -= deltaTime;
        if (m_fleeTimer <= 0.0f)
        {
            // Return to appropriate moving state based on ghost type
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
			case dae::GhostType::Player:
				ghost.SetGhostState(GhostState::m_movingState.get());
            }
            return;
        }

        if (ghost.GetGhostType() == dae::GhostType::Player)
        {
            ghost.SetSpeed(60);
            return;
        }

        // Get grid component and players
        auto* pGrid = ghost.GetGridComponent();
        if (!pGrid) return;

        const auto& players = ghost.GetEnemies();
        if (players.empty()) return;

        // Get ghost's current position
        utils::Vector2f ghostPos = ghost.GetMiddlePosition();
        int ghostCellX = pGrid->GetCellXFromWorld(ghostPos);
        int ghostCellY = pGrid->GetCellYFromWorld(ghostPos);

        // Find all player positions
        std::vector<std::pair<int, int>> playerPositions;
        for (auto* player : players) {
            if (!player) continue;
            utils::Vector2f playerPos = player->GetMiddlePosition();
            int playerCellX = pGrid->GetCellXFromWorld(playerPos);
            int playerCellY = pGrid->GetCellYFromWorld(playerPos);
            playerPositions.emplace_back(playerCellX, playerCellY);
        }

        // Directions: up, right, down, left
        const int dx[] = { 0, 1, 0, -1 };
        const int dy[] = { -1, 0, 1, 0 };

        // Find the direction that maximizes distance from all players
        float maxDistanceSum = -1.0f;
        int bestDir = -1;

        // Grid dimensions
        int width = pGrid->GetWidth();
        int height = pGrid->GetHeight();

        // Evaluate each possible move direction
        for (int dir = 0; dir < 4; dir++) {
            int nextX = ghostCellX + dx[dir];
            int nextY = ghostCellY + dy[dir];

            // Skip if not walkable or out of bounds
            if (nextX < 0 || nextX >= width || nextY < 0 || nextY >= height ||
                !pGrid->IsWalkable(nextX, nextY)) {
                continue;
            }

            // Calculate sum of squared distances to all players (higher is better for fleeing)
            float distanceSum = 0.0f;
            for (const auto& playerPos : playerPositions) {
                int playerX = playerPos.first;
                int playerY = playerPos.second;

                // Manhattan distance (since ghosts move in grid)
                float distance = static_cast<float>(std::abs(playerX - nextX) + std::abs(playerY - nextY));
                distanceSum += distance;
            }

            if (distanceSum > maxDistanceSum) {
                maxDistanceSum = distanceSum;
                bestDir = dir;
            }
        }

        // If no valid direction found, stay put
        if (bestDir == -1) {
            ghost.SetMovementDirection(glm::vec3(0, 0, 0));
            return;
        }

        // Move in the best direction
        glm::vec3 moveDir(0, 0, 0);
        switch (bestDir) {
        case 0: moveDir = glm::vec3(0, -1, 0); break; // Up
        case 1: moveDir = glm::vec3(1, 0, 0); break;  // Right
        case 2: moveDir = glm::vec3(0, 1, 0); break;  // Down
        case 3: moveDir = glm::vec3(-1, 0, 0); break; // Left
        }

		ghost.SetSpeed(60); // Set flee speed

        ghost.SetMovementDirection(moveDir);
    }

    void GhostFleeState::OnEnter(GhostComponent& )
    {
        m_fleeTimer = 6.0f;
    }
    void GhostFleeState::OnExit(GhostComponent& )
    {
    }

    void GhostReturnState::Update(GhostComponent& ghost, float elapsedSec)
    {
        ghost.SetSpeed(200);

        ghost.UpdateReturnSprite(elapsedSec);

        // Get grid component
        auto* pGrid = ghost.GetGridComponent();
        if (!pGrid) return;

        // Get ghost's current position
        utils::Vector2f ghostPos = ghost.GetMiddlePosition();
        int ghostCellX = pGrid->GetCellXFromWorld(ghostPos);
        int ghostCellY = pGrid->GetCellYFromWorld(ghostPos);

        // Target position is cell (13, 11) - the ghost home
        int targetCellX = 13;
        int targetCellY = 11;

        // Check if ghost has reached the target position
        if (ghostCellX == targetCellX && ghostCellY == targetCellY) {
            // Transition back to appropriate moving state based on ghost type
            switch (ghost.GetGhostType()) {
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
			case dae::GhostType::Player:
				ghost.SetGhostState(GhostState::m_movingState.get());
            }
            return;
        }

        // Find the best direction to move toward the target using BFS
        glm::vec3 moveDir = GhostMovingState::FindNextMoveDirection(
            ghostCellX, ghostCellY, targetCellX, targetCellY, pGrid);

        // Move the ghost
        ghost.SetMovementDirection(moveDir);
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

		case dae::GhostType::Player:
            GhostState::m_movingState = std::make_unique<GhostMovingState>();
            GhostState::m_idleState = std::make_unique<GhostIdleState>();
            GhostState::m_fleeState = std::make_unique<GhostFleeState>();
            GhostState::m_returnState = std::make_unique<GhostReturnState>();

            SetGhostState(GhostState::m_idleState.get());
            break;
        }


        SetSrcRect(utils::Rect{ 457, 65, 14, 14 });
        SetDestRect(utils::Rect{ 0, 0, 14*3, 14*3 });
    }

    void GhostComponent::Update(float deltaTime)
    {
		RenderComponent::Update(deltaTime);

        m_ghostState->Update(*this, deltaTime);

		if (m_ghostType == GhostType::Player)
        {
            if (m_movementDir == glm::vec2{ 1,0 })
            {
                    MoveRight(deltaTime);
            
			}
            else if (m_movementDir == glm::vec2{ -1,0 })
            {
                    MoveLeft(deltaTime);
                
            }
            else if (m_movementDir == glm::vec2{ 0,-1 })
            {
                    MoveUp(deltaTime);
                
            }
            else if (m_movementDir == glm::vec2{ 0,1 })
            {
                    MoveDown(deltaTime);
            }
		}
        else
        {
            auto movement = m_movementDir * m_Speed * deltaTime;

            GetOwner()->AddWorldOffset(glm::vec3{ movement.x, movement.y, 0 });
        }

		CheckEnemyCollision();
	}

    void GhostComponent::CheckEnemyCollision()
    {
        auto* pGrid = GetGridComponent();
        if (!pGrid) return;
        const auto& players = GetEnemies();
        if (players.empty()) return;
        // Get ghost's current position
        utils::Vector2f ghostPos = GetMiddlePosition();
        // Check collision with each player
        for (auto* player : players) {
            if (!player) continue;
            // Calculate distance to this player
            utils::Vector2f playerPos = player->GetMiddlePosition();
            float distance = std::sqrtf(std::powf(playerPos.x - ghostPos.x, 2) + std::powf(playerPos.y - ghostPos.y, 2));
            // If distance is less than a threshold, handle collision
            if (m_ghostState == GhostState::m_inkyMovingState.get() || m_ghostState == GhostState::m_pinkyMovingState.get() || m_ghostState == GhostState::m_movingState.get() || m_ghostState == GhostState::m_sueMovingState.get())
            {
                if (distance < 25.0f) { // Adjust threshold as needed
                    player->OnCollision(GetOwner());

                        ResetGhosts();
                    
                }
            }
            else if (m_ghostState == GhostState::m_fleeState.get())
            {
                if (distance < 25.0f) { // Adjust threshold as needed
                    player->EatGhost();
                    SetGhostState(GhostState::m_returnState.get());
                }
			}
            else if (m_ghostState == GhostState::m_pinkyFleeState.get())
            {
                if (distance < 25.0f) { // Adjust threshold as needed
                    player->EatGhost();
                    SetGhostState(GhostState::m_pinkyReturnState.get());
                }
            }
            else if (m_ghostState == GhostState::m_inkyFleeState.get())
                            {
                if (distance < 25.0f) { // Adjust threshold as needed
                    player->EatGhost();
                    SetGhostState(GhostState::m_inkyReturnState.get());
                }
            }
            else if (m_ghostState == GhostState::m_sueFleeState.get())
            {
                if (distance < 25.0f) { // Adjust threshold as needed
                    player->EatGhost();
                    SetGhostState(GhostState::m_sueReturnState.get());
                }
			}
            
		}
    }

    void GhostComponent::ResetGhosts()
    {
		m_pGridComponent->ResetGhosts();
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
    void GhostComponent::UpdateFleeSprite(float deltaTime)
    {
        m_spriteTimer += deltaTime;
        if (m_spriteTimer > m_spriteInterval)
        {
            ++m_spriteIndex;
            m_spriteTimer = 0;
            if (m_spriteIndex >= 2)
                m_spriteIndex = 0;
        }
         switch (m_spriteIndex)
         {
         case 0:
             SetSrcRect(utils::Rect{ 585, 65, 14, 14 });
             break;
         case 1:
             SetSrcRect(utils::Rect{ 633, 65, 14, 14 });
             break;
         default:
             break;
         }
    }
    void GhostComponent::UpdateReturnSprite(float deltaTime)
    {
        m_spriteTimer += deltaTime;
        if (m_spriteTimer > m_spriteInterval)
        {
            ++m_spriteIndex;
            m_spriteTimer = 0;
            if (m_spriteIndex >= 2)
                m_spriteIndex = 0;
        }
        switch (m_spriteIndex)
        {
        case 0:
            SetSrcRect(utils::Rect{ 585, 81, 14, 14 });
            break;
        case 1:
            SetSrcRect(utils::Rect{ 633, 81, 14, 14 });
            break;
        default:
            break;
        }
    }



















    bool GhostComponent::CanMoveUp() const
    {
        if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
            return false;
        int cellSize = m_pGridComponent->GetCellSize() - 1;
        utils::Vector2f middlePos = GetMiddlePosition();
        return m_pGridComponent->GetUpCellFromWorldPos(utils::Vector2f(middlePos.x, middlePos.y + cellSize / 2)).isWalkable;
        //return m_pGridComponent->GetUpCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize/2 +1, middlePos.y + cellSize / 2)).isWalkable && m_pGridComponent->GetUpCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2 -1, middlePos.y + cellSize / 2)).isWalkable;
    }

    bool GhostComponent::CanMoveDown() const
    {
        if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
            return false;

        int cellSize = m_pGridComponent->GetCellSize() - 1;
        utils::Vector2f middlePos = GetMiddlePosition();
        return m_pGridComponent->GetDownCellFromWorldPos(utils::Vector2f(middlePos.x, middlePos.y - cellSize / 2)).isWalkable;
        //return m_pGridComponent->GetDownCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize/2 +1, middlePos.y - cellSize / 2)).isWalkable && m_pGridComponent->GetDownCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2 -1, middlePos.y - cellSize / 2)).isWalkable;
    }

    bool GhostComponent::CanMoveLeft() const
    {
        if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
            return false;

        int cellSize = m_pGridComponent->GetCellSize() - 1;
        utils::Vector2f middlePos = GetMiddlePosition();
        return m_pGridComponent->GetLeftCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2, middlePos.y)).isWalkable;
        //return m_pGridComponent->GetLeftCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2, middlePos.y - cellSize/2 +1)).isWalkable && m_pGridComponent->GetLeftCellFromWorldPos(utils::Vector2f(middlePos.x + cellSize / 2, middlePos.y + cellSize / 2-1)).isWalkable;
    }

    bool GhostComponent::CanMoveRight() const
    {
        if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
            return false;

        int cellSize = m_pGridComponent->GetCellSize() - 1;
        utils::Vector2f middlePos = GetMiddlePosition();
        return m_pGridComponent->GetRightCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize / 2, middlePos.y)).isWalkable;
        //return m_pGridComponent->GetRightCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize / 2, middlePos.y - cellSize/2 +1)).isWalkable && m_pGridComponent->GetRightCellFromWorldPos(utils::Vector2f(middlePos.x - cellSize / 2, middlePos.y + cellSize / 2 -1)).isWalkable;
    }

    void GhostComponent::MoveLeft(float deltaTime)
    {
        if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
        {
            GetOwner()->AddWorldOffset(glm::vec3(-m_Speed * deltaTime, 0, 0));

            if ((GetMiddlePosition().x + m_DestRect.width / 2) < 0)
            {
                SetMiddlePosition(GetMiddlePosition().x + m_pGridComponent->GetWidth() * m_pGridComponent->GetCellSize() + m_DestRect.width / 2, GetMiddlePosition().y);
            }
        }
        else if (CanMoveLeft()) {

            auto midPos = GetMiddlePosition();
            float snapHeight = m_pGridComponent->GetSnapPos(midPos.x, midPos.y).y;

            SetMiddlePosition(midPos.x, snapHeight);

            GetOwner()->AddWorldOffset(glm::vec3(-m_Speed * deltaTime, 0, 0));
        }

    }

    void GhostComponent::MoveRight(float deltaTime)
    {
        if (GetMiddlePosition().x < m_pGridComponent->GetWidth() || GetMiddlePosition().x >(m_pGridComponent->GetWidth() - 1) * m_pGridComponent->GetCellSize())
        {
            GetOwner()->AddWorldOffset(glm::vec3(m_Speed * deltaTime, 0, 0));

            if ((GetMiddlePosition().x - m_DestRect.width / 2) > m_pGridComponent->GetWidth() * m_pGridComponent->GetCellSize())
            {
                SetMiddlePosition(1, GetMiddlePosition().y);
            }
        }
        else if (CanMoveRight())
        {
            auto midPos = GetMiddlePosition();
            float snapHeight = m_pGridComponent->GetSnapPos(midPos.x, midPos.y).y;

            SetMiddlePosition(midPos.x, snapHeight);

            GetOwner()->AddWorldOffset(glm::vec3(m_Speed * deltaTime, 0, 0));
        }
    }

    void GhostComponent::MoveUp(float deltaTime)
    {
        if (CanMoveUp()) {
            auto midPos = GetMiddlePosition();
            float snapWidth = m_pGridComponent->GetSnapPos(midPos.x, midPos.y).x;

            SetMiddlePosition(snapWidth, midPos.y);

            GetOwner()->AddWorldOffset(glm::vec3(0, -m_Speed * deltaTime, 0));
        }
    }

    void GhostComponent::MoveDown(float deltaTime)
    {
        if (CanMoveDown())
        {
            auto midPos = GetMiddlePosition();
            float snapWidth = m_pGridComponent->GetSnapPos(midPos.x, midPos.y).x;

            SetMiddlePosition(snapWidth, midPos.y);

            GetOwner()->AddWorldOffset(glm::vec3(0, m_Speed * deltaTime, 0));
        }
    }
}