#include "NameInputComponent.h"
#include "PacManCharacters.h"
#include "Utils.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "SDL.h"
#include <SDL_ttf.h>

namespace dae
{
	NameInputComponent::NameInputComponent(GameObject& owner, const std::shared_ptr<Font>& font)
		: Component(owner)
		, m_pFont(font)
	{
	}

	NameInputComponent::~NameInputComponent()
	{
		utils::SaveHighScore(g_globalScore ,m_Name);

		g_globalScore = 0; // Reset global score after saving
	}

	void NameInputComponent::Update(float )
	{
	}



	void NameInputComponent::Render(float x, float y) const
	{
		if (!m_pFont)
			return;

		// Render each character with some spacing between them
		const float characterSpacing = 100.0f;

		for (int i = 0; i < 3; ++i)
		{
			const auto& pos = glm::vec2(x + i * characterSpacing, y);

			// Create SDL surface with the character
			const SDL_Color color = { 255, 255, 255, 255 };
			std::string charStr(1, m_Name[i]);

			// Highlight the currently selected character
			SDL_Color textColor = (i == m_CurrentIndex) ? SDL_Color{ 255, 255, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };

			const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), charStr.c_str(), textColor);
			if (surf == nullptr)
			{
				continue;
			}

			// Create texture from surface
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				SDL_FreeSurface(surf);
				continue;
			}

			// Create Texture2D object and render it
			Texture2D tempTexture(texture);
			Renderer::GetInstance().RenderTexture(tempTexture, pos.x, pos.y);

			SDL_FreeSurface(surf);
		}
	}
	void NameInputComponent::goLeft()
	{
		m_CurrentIndex = (m_CurrentIndex - 1 + 3) % 3;
		m_InputCooldown = m_InputDelay;
	}
	void NameInputComponent::goRight()
	{
		m_CurrentIndex = (m_CurrentIndex + 1) % 3;
		m_InputCooldown = m_InputDelay;
	}
	void NameInputComponent::numberUp()
	{
		m_Name[m_CurrentIndex] = (m_Name[m_CurrentIndex] == 'Z') ? 'A' : m_Name[m_CurrentIndex] + 1;
		m_InputCooldown = m_InputDelay;
	}
	void NameInputComponent::numberDown()
	{
		m_Name[m_CurrentIndex] = (m_Name[m_CurrentIndex] == 'A') ? 'Z' : m_Name[m_CurrentIndex] - 1;
		m_InputCooldown = m_InputDelay;
	}
}