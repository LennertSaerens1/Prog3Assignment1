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
    m_DestRect.position.x = GetOwner()->GetWorldPosition().x;
    m_DestRect.position.y = GetOwner()->GetWorldPosition().y;
}

void dae::RenderComponent::FixedUpdate(const float )
{
	
}

void dae::RenderComponent::Render(float x, float y) const
{
    if (m_Texture != nullptr)
    {
        // Check if m_SrcRect and m_DestRect are not (0, 0)
        if (m_SrcRect.width != 0 && m_SrcRect.height != 0 && m_DestRect.width != 0 && m_DestRect.height != 0)
        {
            SDL_Rect src{
                static_cast<int>(m_SrcRect.position.x),
                static_cast<int>(m_SrcRect.position.y),
                static_cast<int>(m_SrcRect.width),
                static_cast<int>(m_SrcRect.height)
            };

            SDL_Rect dest{
                static_cast<int>(m_DestRect.position.x),
                static_cast<int>(m_DestRect.position.y),
                static_cast<int>(m_DestRect.width),
                static_cast<int>(m_DestRect.height)
            };

            dae::Renderer::GetInstance().RenderTexture(*m_Texture, &src, &dest);
        }
        else
        {
            // Default rendering if m_SrcRect or m_DestRect are (0, 0)
            dae::Renderer::GetInstance().RenderTexture(*m_Texture, x, y);
        }
    }
}

void dae::RenderComponent::ImGuiRender()
{
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
