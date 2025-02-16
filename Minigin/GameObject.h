#pragma once
#include <memory>
#include "Transform.h"
#include "iostream"

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject final
	{
	public:
		virtual void Update(const float deltaTime);
		virtual void FixedUpdate(const float fixedTime);
		virtual void Render() const;

		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

        template <typename T, typename... Args>
        void AddComponent(Args&&... args) {
            // Create and add a new component to the vector
            m_Components.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        // Remove a component from the GameObject by pointer (safe removal)
        template <typename T>
        void removeComponent(T* component) {
            auto it = std::find_if(m_Components.begin(), m_Components.end(),
                [component](const std::unique_ptr<Component>& c) {
                    return c.get() == component;  // Check if the raw pointer matches
                });

            if (it != m_Components.end()) {
                m_Components.erase(it);  // Remove the component from the vector
                std::cout << "Component removed.\n";
            }
            else {
                std::cout << "Component not found.\n";
            }
        }

        // Get a component from the GameObject by type (returns nullptr if not found)
        template <typename T>
        T* getComponent() {
            for (auto& component : m_Components) {
                T* specificComponent = dynamic_cast<T*>(component.get());  // Try to cast each component to the requested type
                if (specificComponent) {
                    return specificComponent;  // If the component is found, return it
                }
            }
            return nullptr;  // Return nullptr if the component is not found
        }

        // Check whether a component has been added to the GameObject by type
        template <typename T>
        bool hasComponent() const {
            for (const auto& component : m_Components) {
                if (dynamic_cast<T*>(component.get())) {
                    return true;  // If a component of type T is found, return true
                }
            }
            return false;  // Return false if no component of type T was found
        }

	private:
		Transform m_transform{};
        std::vector<Component*> m_Components;  // Store components
	};
}
