#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(const float deltaTime) {
	for (const auto& component : m_Components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(const float fixedTime) { 
	for (const auto& component : m_Components)
	{
		component->FixedUpdate(fixedTime);
	}
}

void dae::GameObject::Render() const
{
	for (const auto& component :m_Components)
	{
		component->Render(m_transform.GetPosition().x, m_transform.GetPosition().y);
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
