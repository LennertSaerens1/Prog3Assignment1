#include "PelletComponent.h"

dae::PelletComponent::PelletComponent(GameObject& owner)
	: RenderComponent("SpriteSheet.png", owner)
{
	m_SrcRect = utils::Rect{ 0, 0, 16, 16 };
	m_DestRect = utils::Rect{ 0, 0, 16 * 3, 16 * 3 };
}
