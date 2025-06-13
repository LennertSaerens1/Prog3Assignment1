#pragma once
#include "Component.h"
#include "Font.h"

namespace dae
{
	class NameInputComponent: public Component
	{
		public:
		NameInputComponent(GameObject& owner, const std::shared_ptr<Font>& font);
		~NameInputComponent() override;

		void Update(float deltaTime) override;
		void FixedUpdate(const float) override {};
		void Render(float x, float y) const override;
		void ImGuiRender() override {};

		const char* GetName() const { return m_Name; }

		void goLeft();
		void goRight();
		void numberUp();
		void numberDown();
	private:
		char m_Name[3]{'A', 'A', 'A'};
		std::shared_ptr<Font> m_pFont;
		int m_CurrentIndex{ 0 }; // Index of character being edited
		float m_InputCooldown{ 0.f }; // Cooldown for input handling
		const float m_InputDelay{ 0.15f }; // Delay between inputs
	};
}

