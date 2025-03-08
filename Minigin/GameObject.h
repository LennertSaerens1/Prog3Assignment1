#pragma once
#include <memory>
#include "Transform.h"
#include "iostream"
#include "Component.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		virtual void Update(const float deltaTime);
		virtual void FixedUpdate(const float fixedTime);
		virtual void Render() const;
		virtual void ImGuiRender() const;

		void SetWorldPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

        template <typename T, typename... Args>
        void AddComponent(Args&&... args) {
            // Create and add a new component to the vector
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            m_Components.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        // Remove a component from the GameObject by pointer (safe removal)
        template <typename T>
        void removeComponent() {
            auto it = std::remove_if(m_Components.begin(), m_Components.end(),
                [](const std::unique_ptr<Component>& c) {
                    return dynamic_cast<T*>(c.get()) != nullptr;
                });

            if (it != m_Components.end()) {
                m_Components.erase(it, m_Components.end()); // Erase matching elements
                std::cout << "Component(s) of type " << typeid(T).name() << " removed.\n";
            }
            else {
                std::cout << "No component of type " << typeid(T).name() << " found.\n";
            }
        }
        //template <typename T>
        //void removeComponent(T* component) {
        //    if (!component) return; // Null check for safety

        //    auto it = std::remove_if(m_Components.begin(), m_Components.end(),
        //        [component](const std::unique_ptr<Component>& c) {
        //            return c.get() == component;
        //        });

        //    if (it != m_Components.end()) {
        //        m_Components.erase(it, m_Components.end()); // Erase safely
        //        std::cout << "Component removed.\n";
        //    }
        //    else {
        //        std::cout << "Component not found.\n";
        //    }
        //}


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

        void SetParent(GameObject* parent, bool keepWorldPosition);
        
        void SetLocalPosition(const glm::vec3& pos);

        void AddWorldOffset(const glm::vec3& pos);

        void AddLocalOffset(const glm::vec3& pos);

        void SetPositionDirty();

        GameObject* GetParent() const;

        int GetChildCount() const;

        GameObject* GetChildAt(int idx);

        glm::vec3 GetLocalPosition();

        glm::vec3 GetWorldPosition();

        bool IsChild(GameObject* child) const;

        void UpdateWorldPosition();
        

	private:
        void RemoveChild(GameObject* child);
        void AddChild(GameObject* child);

		Transform m_globalTransform{};
		Transform m_localTransform{};
        std::vector< std::unique_ptr<Component>> m_Components;  // Store components
        GameObject* m_parent;
        std::vector<GameObject*> m_children;
        bool m_positionIsDirty;

	};
}
