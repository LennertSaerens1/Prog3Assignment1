#include "GridComponent.h"
#include "Renderer.h"
#include <stdexcept>
#include <iostream>
#include "GameObject.h"
#include <fstream>
#include <stdexcept>


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

    void GridComponent::Update(float )
    {

    }

    void GridComponent::FixedUpdate(float )
    {

    }

    void GridComponent::Render(float x, float y) const
    {
        // Debug rendering logic
        const int cellSize = GetCellSize(); // Size of each grid cell in pixels
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
                }
                if (cell.hasPellet)
                {
                    SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 255, 0, 0, 128); // Red (semi-transparent)
                    SDL_RenderFillRect(renderer.GetSDLRenderer(), &rect);
                }
                if (cell.isGate)
                {
                    SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 0, 0, 255, 128); // Blue (semi-transparent)
                    SDL_RenderFillRect(renderer.GetSDLRenderer(), &rect);
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
    }

    void GridComponent::ImGuiRender()
    {
    }
}
