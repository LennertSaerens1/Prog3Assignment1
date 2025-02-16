#include "FpsComponent.h"
#include <SDL_ttf.h>
#include "Renderer.h"

void FpsComponent::Update(const float deltaTime)
{
    m_Fps = 1.0 / deltaTime;
	m_text = "FPS: " + std::to_string(m_Fps);
}

void FpsComponent::FixedUpdate(const float fixedTime)
{
	fixedTime;
}

void FpsComponent::Render(float x, float y) const
{
	const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_textTexture = std::make_shared<dae::Texture2D>(texture);

	if (m_textTexture != nullptr)
	{
		dae::Renderer::GetInstance().RenderTexture(*m_textTexture, x, y);
	}
}
