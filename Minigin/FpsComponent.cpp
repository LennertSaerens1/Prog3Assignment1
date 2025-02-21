#include "FpsComponent.h"
#include <SDL_ttf.h>
#include "Renderer.h"


dae::FpsComponent::FpsComponent(std::shared_ptr<Font> font, GameObject& owner)
	:TextComponent("0 FPS",font,owner)
	,m_Fps(0)
	,m_elapsedTime{0}
{
	m_textTexture = nullptr;
}

void dae::FpsComponent::Update(const float deltaTime)
{
	m_elapsedTime += deltaTime;

	// Only update the FPS text 3 times per second
	if (m_elapsedTime >= m_rateRefresh) {
		m_Fps = 1.0f / deltaTime;

		// Format FPS with one decimal place
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(1) << m_Fps;
		m_text = stream.str() + " FPS";

		// Reset elapsed time
		m_elapsedTime -= m_rateRefresh;


		//Optimse the rendering of text by only rendering when the text changes in update and just render it in render
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
	}
}

