#pragma once
#include <vector>
#include <algorithm>
#include <memory>

namespace dae
{
	class GameObject;

	class Component {
		GameObject* m_pOwner{}; // Pointer to the GameObject that owns this component

	public:
		virtual ~Component() {} // Virtual destructor for proper cleanup
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update(float deltaTime) = 0; // Pure virtual function for updates
		virtual void FixedUpdate(const float fixedTime) = 0; // Pure virtual function for fixed updates
		virtual void Render(float x, float y) const = 0; // Pure virtual function for rendering
		virtual void ImGuiRender() = 0;

	protected:
		explicit Component(GameObject& owner); // Constructor that takes a GameObject as parameter
		GameObject* GetOwner() const { return m_pOwner; } // Get the GameObject that owns this component
	};
}

