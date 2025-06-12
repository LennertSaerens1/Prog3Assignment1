#pragma once
#include "Component.h"
#include "Pellet.h"
#include <vector>
#include "Utils.h"
#include <string>
#include "PacManCharacters.h"

namespace dae
{
    class PacManCharacter;

    class GridComponent : public Component
    {
    public:
        struct Cell
        {
            bool isWalkable{ false };   // Can Pac-Man walk here?
            bool hasPowerPill{ false }; // Does this cell contain a power pill?
            bool hasPellet{ false };    // Does this cell contain a pellet?
            bool isGate{ false };       // Is this cell a gate to the other side?
			//std::unique_ptr<Pellet> pPellet{ nullptr }; // Pointer to the pellet object in this cell
			Pellet* pPellet{ nullptr }; // Pointer to the pellet object in this cell
			PowerPellet* pPowerPill{ nullptr }; // Pointer to the power pill object in this cell
        };

        GridComponent(GameObject& owner, int width, int height, int gridWidthInPixels, int gridHeightInPixels);

        // Accessors
        const Cell& GetCell(int x, int y) const;
        const Cell& GetCellFromWorldPos(const utils::Vector2f& worldPos) const;

		int GetCellSize() const
		{
            const int cellSizeX = m_GridWidthInPixels / m_Width;  // Cell width in pixels
			return cellSizeX;
		}

        const Cell& GetRightCellFromWorldPos(const utils::Vector2f& worldPos) const;
		const Cell& GetLeftCellFromWorldPos(const utils::Vector2f& worldPos) const;
		const Cell& GetUpCellFromWorldPos(const utils::Vector2f& worldPos) const;
		const Cell& GetDownCellFromWorldPos(const utils::Vector2f& worldPos) const;

		const int GetCellXFromWorld(const utils::Vector2f& worldPos) const;
		const int GetCellYFromWorld(const utils::Vector2f& worldPos) const;

		void PickUpPellet(int x, int y);
		void PickUpPellet(Cell& cell);
		void PickUpPowerPill(int x, int y);
		void PickUpPowerPill(Cell& cell);

        void SetCell(int x, int y, const Cell& cell);

        // Utility methods
        bool IsWalkable(int x, int y) const;
        bool HasPowerPill(int x, int y) const;
        bool HasPellet(int x, int y) const;
        bool IsGate(int x, int y) const;

		float GetPowerActiveTime() const { return m_PowerActiveTime; }

        utils::Vector2f GetWorldCoordinates(int x, int y) const;
        utils::Vector2f GetWorldCoordinatesMiddle(int x, int y) const;

        utils::Vector2f GetSnapPos(float x, float y) const;

        void LoadGridFromFile(const std::string& filepath);

		void AddAllPelletsToScene();

		void NextLevel();

        void GetPacMan(PacManCharacter* pacMan)
        {
            m_pPacMan = pacMan;
        }

        // Grid dimensions
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        void Update(float deltaTime) override;
        void FixedUpdate(float fixedTime) override;
        void Render(float x, float y) const override;
        void ImGuiRender() override;

    private:
		int m_Level{ 1 };
        int m_Width;
        int m_Height;
		int m_GridWidthInPixels;  // Width of the grid in pixels
		int m_GridHeightInPixels; // Height of the grid in pixels

        std::vector<std::vector<Cell>> m_Grid; // 2D grid of cells
		std::vector<std::unique_ptr<Pellet>> m_pPellets; // Vector of unique pointers to Pellet objects
		std::vector<std::unique_ptr<PowerPellet>> m_pPowerPills; // Vector of unique pointers to PowerPellet objects

		PacManCharacter* m_pPacMan{ nullptr };

		float m_PowerActiveTime{ 0.0f };
    };
}
