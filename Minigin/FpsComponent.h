#pragma once
#include "TextObject.h"
#include <string>
#include <chrono>
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	class FpsComponent : public TextComponent
	{
	public:
		void Update(const float deltaTime) override;

		float GetFps() const { return m_Fps; }

		FpsComponent(std::shared_ptr<Font> font, GameObject& owner);
		virtual ~FpsComponent() = default;
		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;
	private:
		float m_Fps;
		float m_elapsedTime;
		const float m_rateRefresh{ (1.0f / 10.0f) };
	};
}

