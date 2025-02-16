#pragma once
#include "Component.h"
#include <string>
#include <chrono>
#include "Font.h"
#include "Texture2D.h"

class FpsComponent : private Component
{
	void Update(const float deltaTime) override;
	void FixedUpdate(const float fixedTime) override;
	void Render(float x, float y) const override;

	double GetFps() const { return m_Fps; }

	FpsComponent();
	virtual ~FpsComponent() = default;
	FpsComponent(const FpsComponent& other) = delete;
	FpsComponent(FpsComponent&& other) = delete;
	FpsComponent& operator=(const FpsComponent& other) = delete;
	FpsComponent& operator=(FpsComponent&& other) = delete;
private:
	std::chrono::high_resolution_clock::time_point m_LastTime;
	double m_Fps;
	std::string m_text;
	std::shared_ptr<dae::Font> m_font;
	mutable std::shared_ptr<dae::Texture2D> m_textTexture;
};

