#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

RenderComponent::RenderComponent()
	: m_needsUpdate(false)
	, m_Texture(nullptr)
{
}

RenderComponent::RenderComponent(const std::string& filePath)
	: m_needsUpdate(false)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filePath);
}

void RenderComponent::Update(const float deltaTime)
{
	deltaTime;
}

void RenderComponent::FixedUpdate(const float fixedTime)
{
	fixedTime;
}

void RenderComponent::Render(float x, float y) const
{
	if (m_Texture != nullptr)
	{
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, x, y);
	}
}

void RenderComponent::SetTexture(const std::string& filePath)
{
	if (m_Texture == nullptr)
	{
		m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filePath);
	}
	else
	{
		m_Texture.reset();
		m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filePath);
	}
	
}
