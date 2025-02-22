#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

dae::RenderComponent::RenderComponent(GameObject& owner)
	: Component::Component(owner)
	,m_needsUpdate(false)
	, m_Texture(nullptr)
{
}

dae::RenderComponent::RenderComponent(const std::string& filePath, GameObject& owner)
	: Component::Component(owner)
	, m_needsUpdate(false)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filePath);
}

void dae::RenderComponent::Update(const float )
{
	
}

void dae::RenderComponent::FixedUpdate(const float )
{
	
}

void dae::RenderComponent::Render(float x, float y) const
{
	if (m_Texture != nullptr)
	{
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, x, y);
	}
}

void dae::RenderComponent::Render(float x, float y, float Width, float Height) const
{
	if (m_Texture != nullptr)
	{
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, x, y, Width, Height);
	}
}

void dae::RenderComponent::Render(float x, float y, float srcX, float srcY, float srcWidth, float srcHeight) const
{
	if (m_Texture != nullptr)
	{
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, x, y,srcX,srcY, srcWidth, srcHeight);
	}
}

void dae::RenderComponent::SetTexture(const std::string& filePath)
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
