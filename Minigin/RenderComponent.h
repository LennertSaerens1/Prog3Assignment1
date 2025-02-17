#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"
#include "Font.h"

namespace dae
{


	class RenderComponent : public Component
	{
	public:
		void Update(const float deltaTime) override;
		void FixedUpdate(const float fixedTime) override;
		void Render(float x, float y) const override;

		void SetTexture(const std::string& filePath);

		RenderComponent(GameObject& owner);
		RenderComponent(const std::string& filePath, GameObject& owner);
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
	private:
		bool m_needsUpdate;
		std::shared_ptr<dae::Texture2D> m_Texture;

	};
}

