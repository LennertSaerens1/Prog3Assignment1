#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"
#include "Font.h"

class RenderComponent : private Component
{
	void Update(const float deltaTime) override;
	void FixedUpdate(const float fixedTime) override;
	void Render(float x, float y) const override;

	void SetTexture(const std::string& filePath);

	RenderComponent(const std::string& text, std::shared_ptr<dae::Font> font);
	virtual ~RenderComponent() = default;
	RenderComponent(const RenderComponent& other) = delete;
	RenderComponent(RenderComponent&& other) = delete;
	RenderComponent& operator=(const RenderComponent& other) = delete;
	RenderComponent& operator=(RenderComponent&& other) = delete;
private:
	bool m_needsUpdate;
	std::shared_ptr<dae::Texture2D> m_Texture;

};

