#pragma once
#include "RenderComponent.h"
#include "SceneManager.h"
#include "Utils.h"
namespace dae
{
	class GridComponent;

	class Pellet
	{
	public:
		Pellet(float x, float y, float width, float height);
		void Render() const;
		/*void Update(const float deltaTime) override;
		void FixedUpdate(const float fixedTime) override;
		void ImGuiRender() override;*/
		bool IsActive() const { return m_Active; }
		void SetActive(bool active) { m_Active = active; }
	protected:
		std::shared_ptr<dae::Texture2D> m_Texture;
		utils::Rect m_SrcRect;
		utils::Rect m_DestRect;
		bool m_Active{ true };
	};

	class PowerPellet : public Pellet
	{
	public:
		PowerPellet(float x, float y, float width, float height)
			: Pellet(x, y, width, height)
		{
			m_SrcRect = utils::Rect{ 8, 16, 8, 8 };
		}
		void Render() const
		{
			Pellet::Render();
		}
	};
}

