#include "GridComponent.h"
#include "Renderer.h"
#include <stdexcept>
#include <iostream>
#include "GameObject.h"
#include "PacManCharacters.h"
#include "GhostComponent.h"
#include <fstream>
#include <stdexcept>
#include "ServiceLocator.h"
#include "NameInputComponent.h"

constexpr bool SHOULD_DEBUG_GRID = false;

namespace dae
{
    GridComponent::GridComponent(GameObject& owner, int width, int height, int gridWidthInPixels, int gridHeightInPixels)
        : Component(owner), m_Width(width), m_Height(height), m_GridWidthInPixels(gridWidthInPixels), m_GridHeightInPixels(gridHeightInPixels), m_Grid(height, std::vector<Cell>(width))
    {
    }

    const GridComponent::Cell& GridComponent::GetCell(int x, int y) const
    {
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
        {
            throw std::out_of_range("Grid coordinates out of bounds");
        }
        return m_Grid[y][x];
    }

    const GridComponent::Cell& GridComponent::GetCellFromWorldPos(const utils::Vector2f& worldPos) const
    {
		const int cellSize = GetCellSize(); // Size of each grid cell in pixels
		int x = static_cast<int>((worldPos.x - GetOwner()->GetWorldPosition().x) / cellSize);
		int y = static_cast<int>((worldPos.y - GetOwner()->GetWorldPosition().y) / cellSize);
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			throw std::out_of_range("Grid coordinates out of bounds");
		}
		return m_Grid[y][x];
        
    }

    const GridComponent::Cell& GridComponent::GetRightCellFromWorldPos(const utils::Vector2f& worldPos) const
    {
        const int cellSize = GetCellSize(); // Size of each grid cell in pixels
        int x = static_cast<int>((worldPos.x - GetOwner()->GetWorldPosition().x) / cellSize);
        int y = static_cast<int>((worldPos.y - GetOwner()->GetWorldPosition().y) / cellSize);
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
        {
            throw std::out_of_range("Grid coordinates out of bounds");
        }
        return m_Grid[y][x+1];
    }

    const GridComponent::Cell& GridComponent::GetLeftCellFromWorldPos(const utils::Vector2f& worldPos) const
    {
        const int cellSize = GetCellSize(); // Size of each grid cell in pixels
        int x = static_cast<int>((worldPos.x - GetOwner()->GetWorldPosition().x) / cellSize);
        int y = static_cast<int>((worldPos.y - GetOwner()->GetWorldPosition().y) / cellSize);
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
        {
            throw std::out_of_range("Grid coordinates out of bounds");
        }
        return m_Grid[y][x-1];
    }

    const GridComponent::Cell& GridComponent::GetUpCellFromWorldPos(const utils::Vector2f& worldPos) const
    {
        const int cellSize = GetCellSize(); // Size of each grid cell in pixels
        int x = static_cast<int>((worldPos.x - GetOwner()->GetWorldPosition().x) / cellSize);
        int y = static_cast<int>((worldPos.y - GetOwner()->GetWorldPosition().y) / cellSize);
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
        {
            throw std::out_of_range("Grid coordinates out of bounds");
        }
        return m_Grid[y-1][x];
    }

    const GridComponent::Cell& GridComponent::GetDownCellFromWorldPos(const utils::Vector2f& worldPos) const
    {
        const int cellSize = GetCellSize(); // Size of each grid cell in pixels
        int x = static_cast<int>((worldPos.x - GetOwner()->GetWorldPosition().x) / cellSize);
        int y = static_cast<int>((worldPos.y - GetOwner()->GetWorldPosition().y) / cellSize);
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
        {
            throw std::out_of_range("Grid coordinates out of bounds");
        }
        return m_Grid[y+1][x];
    }

    const int GridComponent::GetCellXFromWorld(const utils::Vector2f& worldPos) const
    {
        const int cellSize = GetCellSize(); // Size of each grid cell in pixels
        int x = static_cast<int>((worldPos.x - GetOwner()->GetWorldPosition().x) / cellSize);

        return x;
    }

    const int GridComponent::GetCellYFromWorld(const utils::Vector2f& worldPos) const
    {
        const int cellSize = GetCellSize(); // Size of each grid cell in pixels
        int y = static_cast<int>((worldPos.y - GetOwner()->GetWorldPosition().y) / cellSize);

		return y;
    }

    void GridComponent::PickUpPellet(int x, int y)
    {
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
	    {
	    	throw std::out_of_range("Grid coordinates out of bounds");
	    }
		Cell& cell = m_Grid[y][x];
		if (cell.hasPellet && cell.pPellet != nullptr)
		{
			cell.pPellet->SetActive(false);
			cell.hasPellet = false;
			cell.pPellet = nullptr; // Clear the pointer to the pellet

            m_pPacMan->AddScore(10);
		}
    }

    void GridComponent::PickUpPellet(Cell& cell)
    {
		if (cell.hasPellet && cell.pPellet != nullptr)
		{
			cell.pPellet->SetActive(false);
			cell.hasPellet = false;
			cell.pPellet = nullptr; // Clear the pointer to the pellet

            m_pPacMan->AddScore(10);

		}
    }

    void GridComponent::PickUpPowerPill(int x, int y)
    {
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			throw std::out_of_range("Grid coordinates out of bounds");
		}
		Cell& cell = m_Grid[y][x];
        if (cell.hasPowerPill && cell.pPowerPill != nullptr)
        {
            cell.pPowerPill->SetActive(false);
            cell.hasPowerPill = false;
            cell.pPowerPill = nullptr; // Clear the pointer to the power pill

            m_pPacMan->AddScore(50);

            m_pPacMan->SetEatGhostTimer(6.f);

            for (auto& ghost : m_pGhosts)
            {
                switch (ghost->GetGhostType())
                {
                    case GhostType::Blinky:
                        if (ghost->GetGhostState() == GhostState::m_movingState.get() || ghost->GetGhostState() == GhostState::m_fleeState.get())
                        {
                            ghost->SetGhostState(GhostState::m_fleeState.get());
						}
						break;
                    case GhostType::Pinky:
                        if (ghost->GetGhostState() == GhostState::m_pinkyMovingState.get() || ghost->GetGhostState() == GhostState::m_pinkyFleeState.get())
                        {
                            ghost->SetGhostState(GhostState::m_pinkyFleeState.get());
                        }
						break;
                        case GhostType::Inky:
                            if (ghost->GetGhostState() == GhostState::m_inkyMovingState.get() || ghost->GetGhostState() == GhostState::m_inkyFleeState.get())
                            {
                                ghost->SetGhostState(GhostState::m_inkyFleeState.get());
                            }
					break;
                    case GhostType::Sue:
                        if (ghost->GetGhostState() == GhostState::m_sueMovingState.get() || ghost->GetGhostState() == GhostState::m_sueFleeState.get())
                        {
                            ghost->SetGhostState(GhostState::m_sueFleeState.get());
                        }
						break;
                }
			}

			m_PowerActiveTime = 5.0f;
        }
    }

    void GridComponent::PickUpPowerPill(Cell& cell)
    {
        if (cell.hasPowerPill && cell.pPowerPill != nullptr)
        {
            cell.pPowerPill->SetActive(false);
            cell.hasPowerPill = false;
            cell.pPowerPill = nullptr; // Clear the pointer to the power pill

            m_pPacMan->AddScore(50);

			m_pPacMan->SetEatGhostTimer(6.f);
            
            for (auto& ghost : m_pGhosts)
            {
                switch (ghost->GetGhostType())
                {
                case GhostType::Blinky:
                    if (ghost->GetGhostState() == GhostState::m_movingState.get() || ghost->GetGhostState() == GhostState::m_fleeState.get())
                    {
                        ghost->SetGhostState(GhostState::m_fleeState.get());
                    }
                    break;
                case GhostType::Pinky:
                    if (ghost->GetGhostState() == GhostState::m_pinkyMovingState.get() || ghost->GetGhostState() == GhostState::m_pinkyFleeState.get())
                    {
                        ghost->SetGhostState(GhostState::m_pinkyFleeState.get());
                    }
                    break;
                case GhostType::Inky:
                    if (ghost->GetGhostState() == GhostState::m_inkyMovingState.get() || ghost->GetGhostState() == GhostState::m_inkyFleeState.get())
                    {
                        ghost->SetGhostState(GhostState::m_inkyFleeState.get());
                    }
                    break;
                case GhostType::Sue:
                    if (ghost->GetGhostState() == GhostState::m_sueMovingState.get() || ghost->GetGhostState() == GhostState::m_sueFleeState.get())
                    {
                        ghost->SetGhostState(GhostState::m_sueFleeState.get());
                    }
                    break;
                }
            }

            m_PowerActiveTime = 5.0f;
        }
    }

    void GridComponent::SetCell(int x, int y, const Cell& cell)
    {
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
        {
            throw std::out_of_range("Grid coordinates out of bounds");
        }
        m_Grid[y][x] = cell;
    }

    bool GridComponent::IsWalkable(int x, int y) const
    {
        return GetCell(x, y).isWalkable;
    }

    bool GridComponent::HasPowerPill(int x, int y) const
    {
        return GetCell(x, y).hasPowerPill;
    }

    bool GridComponent::HasPellet(int x, int y) const
    {
        return GetCell(x, y).hasPellet;
    }

    bool GridComponent::IsGate(int x, int y) const
    {
        return GetCell(x, y).isGate;
    }

    utils::Vector2f GridComponent::GetWorldCoordinates(int x, int y) const
    {
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
        {
            throw std::out_of_range("Grid coordinates out of bounds");
        }

        const int cellSize = GetCellSize(); // Size of each grid cell in pixels
        utils::Vector2f worldPosition;
		auto world = GetOwner()->GetWorldPosition();
        worldPosition.x = x * cellSize + world.x;
        worldPosition.y = y * cellSize + world.y;
        return worldPosition;
    }

    utils::Vector2f GridComponent::GetWorldCoordinatesMiddle(int x, int y) const
    {
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
        {
            throw std::out_of_range("Grid coordinates out of bounds");
        }

        const int cellSize = GetCellSize(); // Size of each grid cell in pixels
        utils::Vector2f worldPosition;
        auto world = GetOwner()->GetWorldPosition();
        worldPosition.x = x * cellSize + world.x + cellSize/2;
        worldPosition.y = y * cellSize + world.y + cellSize/2;
        return worldPosition;
    }

    utils::Vector2f GridComponent::GetSnapPos(float x, float y) const
    {
        const int cellSize = GetCellSize();
        auto world = GetOwner()->GetWorldPosition();

        // Convert world position to grid cell indices
        int cellX = static_cast<int>((x - world.x) / cellSize);
        int cellY = static_cast<int>((y - world.y) / cellSize);

        // Clamp to grid bounds
        if (cellX < 0) cellX = 0;
        if (cellX >= m_Width) cellX = m_Width - 1;
        if (cellY < 0) cellY = 0;
        if (cellY >= m_Height) cellY = m_Height - 1;

        // Get the center of the cell in world coordinates
        return GetWorldCoordinatesMiddle(cellX, cellY);
    }


    

    void GridComponent::Update(float deltaTime)
    {
        // Remove inactive pellets from the vector and clear their pointers in the grid
        auto it = std::remove_if(m_pPellets.begin(), m_pPellets.end(),
            [this](const std::unique_ptr<Pellet>& pellet)
            {
                if (!pellet->IsActive())
                {
                    // Find and clear the pointer in the grid
                    for (int row = 0; row < m_Height; ++row)
                    {
                        for (int col = 0; col < m_Width; ++col)
                        {
                            if (m_Grid[row][col].pPellet == pellet.get())
                            {
                                m_Grid[row][col].pPellet = nullptr;
                                m_Grid[row][col].hasPellet = false;
                            }
                        }
                    }
                    return true; // Remove from vector
                }
                return false; // Keep in vector
            });
        m_pPellets.erase(it, m_pPellets.end());

        // Remove inactive power pills from the vector and clear their pointers in the grid
        auto it2 = std::remove_if(m_pPowerPills.begin(), m_pPowerPills.end(),
            [this](const std::unique_ptr<PowerPellet>& powerPill)
            {
                if (!powerPill->IsActive())
                {
                    // Find and clear the pointer in the grid
                    for (int row = 0; row < m_Height; ++row)
                    {
                        for (int col = 0; col < m_Width; ++col)
                        {
                            if (m_Grid[row][col].pPowerPill == powerPill.get())
                            {
                                m_Grid[row][col].pPowerPill = nullptr;
                                m_Grid[row][col].hasPowerPill = false;
                            }
                        }
                    }
                    return true; // Remove from vector
                }
                return false; // Keep in vector
            });
        m_pPowerPills.erase(it2, m_pPowerPills.end());

        if (m_PowerActiveTime > 0.0f)
        {
            m_PowerActiveTime -= deltaTime;
        }

		//Check if all pellets and power pills are collected
        bool allPelletsCollected = true;
        for (const auto& row : m_Grid)
        {
            for (const auto& cell : row)
            {
                if (cell.hasPellet || cell.hasPowerPill)
                {
                    allPelletsCollected = false;
                    break;
                }
            }
            if (!allPelletsCollected) break;
        }
        if (allPelletsCollected)
        {
            m_pPacMan->LevelUp();

            NextLevel();
		}
    }

    void GridComponent::FixedUpdate(float )
    {

    }

    void GridComponent::Render(float x, float y) const
    {
        const int cellSize = GetCellSize(); 

        for (int row = 0; row < m_Height; ++row)
        {
            for (int col = 0; col < m_Width; ++col)
            {
                const Cell& cell = m_Grid[row][col];
                if (cell.hasPowerPill)
                {
                    if (cell.pPowerPill)
                    {
                        cell.pPowerPill->Render();
                    }
                }
                if (cell.hasPellet)
                {
                    if (cell.pPellet)
                    {
                        cell.pPellet->Render();
                    }
                }

            }
        }

        if (SHOULD_DEBUG_GRID) // Debug rendering logic
        {
            auto& renderer = dae::Renderer::GetInstance();

            // Enable blending for transparency
            SDL_SetRenderDrawBlendMode(renderer.GetSDLRenderer(), SDL_BLENDMODE_BLEND);

            for (int row = 0; row < m_Height; ++row)
            {
                for (int col = 0; col < m_Width; ++col)
                {
                    const Cell& cell = m_Grid[row][col];
                    SDL_Rect rect{
                        static_cast<int>(x + col * cellSize),
                        static_cast<int>(y + row * cellSize),
                        static_cast<int>(cellSize),
                        static_cast<int>(cellSize)
                    };

                    // Draw the cell border
                    SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 255, 255, 255, 255); // White
                    SDL_RenderDrawRect(renderer.GetSDLRenderer(), &rect);

                    // Fill the cell based on its properties
                    if (cell.isWalkable)
                    {
                        SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 0, 255, 0, 128); // Green (semi-transparent)
                        SDL_RenderFillRect(renderer.GetSDLRenderer(), &rect);
                    }
                    if (cell.hasPowerPill)
                    {
                        SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 255, 255, 0, 128); // Yellow (semi-transparent)
                        SDL_RenderFillRect(renderer.GetSDLRenderer(), &rect);

                        if (cell.pPowerPill)
                        {
                            cell.pPowerPill->Render();
                        }
                    }
                    if (cell.hasPellet)
                    {
                        SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 255, 0, 0, 128); // Red (semi-transparent)
                        SDL_RenderFillRect(renderer.GetSDLRenderer(), &rect);

                        if (cell.pPellet)
                        {
                            cell.pPellet->Render();
                        }
                    }
                    if (cell.isGate)
                    {
                        SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 0, 0, 255, 128); // Blue (semi-transparent)
                        SDL_RenderFillRect(renderer.GetSDLRenderer(), &rect);
                    }
                }
            }
        }
    }

    void GridComponent::LoadGridFromFile(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open grid file: " + filepath);
        }

        std::vector<std::vector<Cell>> newGrid;
        std::string line;
        int row = 0;

        while (std::getline(file, line))
        {
            std::vector<Cell> gridRow;
            for (size_t col = 0; col < line.size(); ++col)
            {
                Cell cell;
                switch (line[col])
                {
                case 'W': // Wall
                    cell.isWalkable = false;
                    break;
                case 'P': // Power Pill
                    cell.isWalkable = true;
                    cell.hasPowerPill = true;
                    break;
                case '.': // Pellet
                    cell.isWalkable = true;
                    cell.hasPellet = true;
                    break;
                case ' ': // Walkable space
                    cell.isWalkable = true;
                    break;
				case 'G': // Gate
					cell.isWalkable = true;
					cell.isGate = true;
                    break;
                default:
                    throw std::runtime_error("Invalid character in grid file at row " + std::to_string(row) + ", col " + std::to_string(col));
                }
                gridRow.push_back(cell);
            }
            newGrid.push_back(gridRow);
            ++row;
        }

        // Validate grid dimensions
        if (!newGrid.empty())
        {
            int newWidth = static_cast<int>(newGrid[0].size());
            int newHeight = static_cast<int>(newGrid.size());
            for (const auto& gridRow : newGrid)
            {
                if (gridRow.size() != static_cast<size_t>(newWidth))
                {
                    throw std::runtime_error("Inconsistent row lengths in grid file");
                }
            }

            // Update the grid
            m_Width = newWidth;
            m_Height = newHeight;
            m_Grid = std::move(newGrid);
        }
        else
        {
            throw std::runtime_error("Grid file is empty");
        }

		AddAllPelletsToScene();
    }

    void GridComponent::AddAllPelletsToScene()
    {
        // Clear previous pellets and pointers
        m_pPellets.clear();
        for (int row = 0; row < m_Height; ++row)
        {
            for (int col = 0; col < m_Width; ++col)
            {
                m_Grid[row][col].pPellet = nullptr;
            }
        }

        // Add new pellets
        for (int row = 0; row < m_Height; ++row)
        {
            for (int col = 0; col < m_Width; ++col)
            {
                if (m_Grid[row][col].hasPellet)
                {
					auto worldPos = GetWorldCoordinatesMiddle(col, row);
                    auto pellet = std::make_unique<Pellet>(
                        static_cast<float>(worldPos.x) - m_Width/2,
                        static_cast<float>(worldPos.y) - m_Height/2,
                        static_cast<float>(GetWidth()), static_cast<float>(GetHeight())
                    );
                    m_pPellets.push_back(std::move(pellet));
					m_Grid[row][col].pPellet = m_pPellets[m_pPellets.size() - 1].get(); // Store the pointer to the pellet in the cell
					//std::cout << "Pellet pointer is: " << m_pPellets[m_pPellets.size() - 1].get() << std::endl;
                }
            }
        }

		// clear the power pill pointers
        for (int row = 0; row < m_Height; ++row)
        {
            for (int col = 0; col < m_Width; ++col)
            {
                m_Grid[row][col].pPowerPill = nullptr;
            }
        }

		// Add new power pills
		for (int row = 0; row < m_Height; ++row)
		{
			for (int col = 0; col < m_Width; ++col)
			{
				if (m_Grid[row][col].hasPowerPill)
				{
					auto worldPos = GetWorldCoordinatesMiddle(col, row);
					auto powerPill = std::make_unique<PowerPellet>(
						static_cast<float>(worldPos.x) - m_Width / 2,
						static_cast<float>(worldPos.y) - m_Height / 2,
						static_cast<float>(GetWidth()), static_cast<float>(GetWidth())
					);
					m_pPowerPills.push_back(std::move(powerPill));
					m_Grid[row][col].pPowerPill = m_pPowerPills[m_pPowerPills.size() - 1].get(); // Store the pointer to the power pill in the cell
				}
			}
		}

    }

    void GridComponent::NextLevel()
    {
		++m_Level;

        switch (m_Level)
        {
        case 1:
			LoadGridFromFile("Level1.txt");
            GetOwner()->getComponent<dae::RenderComponent>()->SetSrcRect(utils::Rect{ 228, 0, 224, 248 });
            break;
		case 2:
            LoadGridFromFile("Level2.txt");
            GetOwner()->getComponent<dae::RenderComponent>()->SetSrcRect(utils::Rect{ 228, 248 * 1, 224, 248 });

			break;
        case 3:
            LoadGridFromFile("Level3.txt");
            GetOwner()->getComponent<dae::RenderComponent>()->SetSrcRect(utils::Rect{ 228, 248 * 2, 224, 248 });

			break;
        default:
			m_pPacMan->SetEndScreen();
			return; // No more levels to load
        }

        auto& soundSystem = dae::ServiceLocator::GetSoundSystem();
        soundSystem.StopAllSounds();

		ResetGhosts();
    }

    void GridComponent::ResetGhosts()
    {
        for (auto& ghost : m_pGhosts)
        {
            utils::Vector2f middlePos{};
            switch (ghost->GetGhostType())
            {
            case GhostType::Blinky:
                ghost->SetGhostState(GhostState::m_idleState.get());
				GhostState::m_movingState->SetShouldReset(true);
                middlePos = GetWorldCoordinatesMiddle(13, 11);
                ghost->SetMiddlePosition(middlePos.x, middlePos.y); // Reset to Blinky's starting position
				break;
            case GhostType::Pinky:
                ghost->SetGhostState(GhostState::m_pinkyIdleState.get());
				GhostState::m_pinkyMovingState->SetShouldReset(true);
                middlePos = GetWorldCoordinatesMiddle(11, 14);

				ghost->SetMiddlePosition(middlePos.x, middlePos.y); // Reset to Pinky's starting position
                break;
            case GhostType::Inky:
				ghost->SetGhostState(GhostState::m_inkyIdleState.get());
				GhostState::m_inkyMovingState->SetShouldReset(true);
                middlePos = GetWorldCoordinatesMiddle(13, 14);

				ghost->SetMiddlePosition(middlePos.x, middlePos.y); // Reset to Inky's starting position
                break;
	        case GhostType::Sue:
				ghost->SetGhostState(GhostState::m_sueIdleState.get());
				GhostState::m_sueMovingState->SetShouldReset(true);
                middlePos = GetWorldCoordinatesMiddle(15, 14);
				ghost->SetMiddlePosition(middlePos.x, middlePos.y); // Reset to Sue's starting position
                break;
            }
        }
	}

    void GridComponent::ImGuiRender()
    {
    }
}
