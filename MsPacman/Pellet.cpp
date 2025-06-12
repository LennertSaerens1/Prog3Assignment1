#include "Pellet.h"
#include "GridComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

dae::Pellet::Pellet(float x, float y, float width, float height)
{
    m_Texture = dae::ResourceManager::GetInstance().LoadTexture("SpriteSheet.png");
    m_DestRect = utils::Rect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) };
    m_SrcRect = utils::Rect{ 8, 8, 8, 8 };
}

void dae::Pellet::Render() const
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
    }
}
