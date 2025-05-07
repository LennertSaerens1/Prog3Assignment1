#pragma once
#include "RenderComponent.h"
#include "SceneManager.h"
namespace dae
{

	class PelletComponent : public RenderComponent
	{
	public:
		PelletComponent(GameObject& owner);
		/*void Update(const float deltaTime) override;
		void FixedUpdate(const float fixedTime) override;
		void Render(float x, float y) const override;
		void ImGuiRender() override;*/
	};
}

