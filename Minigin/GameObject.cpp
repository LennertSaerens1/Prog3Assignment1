#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
    if (IsChild(parent) || parent == this || m_parent == parent)
        return;
    if (parent == nullptr)
        SetLocalPosition(GetWorldPosition());
    else
    {
        if (keepWorldPosition)
            SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
        SetPositionDirty();
    }
    if (m_parent) m_parent->RemoveChild(this);
    m_parent = parent;
    if (m_parent) m_parent->AddChild(this);
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
    m_localTransform.SetPosition(pos.x, pos.y, pos.z);
    SetPositionDirty();
}

void dae::GameObject::SetPositionDirty()
{
    m_positionIsDirty = true;
    for (auto child : m_children)
    {
        child->SetPositionDirty();
    }
    //Children moeten ook dirty zijn!
}

dae::GameObject* dae::GameObject::GetParent() const
{
    return m_parent;
}

int dae::GameObject::GetChildCount() const
{
    return static_cast<int>(m_children.size());
}

dae::GameObject* dae::GameObject::GetChildAt(int idx)
{
    if (idx < 0 || idx >= static_cast<int>(m_children.size())) {
        std::cout << "Invalid child index.\n";
        return nullptr;
    }
    return m_children[idx];
}

glm::vec3 dae::GameObject::GetLocalPosition()
{
    return m_localTransform.GetPosition();
}

glm::vec3 dae::GameObject::GetWorldPosition()
{
    if (m_positionIsDirty)
        UpdateWorldPosition();
    return m_globalTransform.GetPosition();

}

void dae::GameObject::UpdateWorldPosition()
{
    if (m_positionIsDirty)
    {
        if (m_parent == nullptr)
        {
            auto pos = m_localTransform.GetPosition();
            m_globalTransform.SetPosition(pos);
        }
        else
        {
            m_globalTransform.SetPosition( m_parent->GetWorldPosition() + m_localTransform.GetPosition());
        }
    }
    m_positionIsDirty = false;
}


bool dae::GameObject::IsChild(GameObject* child) const
{
    return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
}

void dae::GameObject::RemoveChild(GameObject* child)
{
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}

void dae::GameObject::AddChild(GameObject* child)
{
    //Mag enkel omdat het private is!
    m_children.emplace_back(child);
}


void dae::GameObject::Update(const float deltaTime) {
	for (const auto& component : m_Components)
	{
		component->Update(deltaTime);
	}
    UpdateWorldPosition();
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
		component->Render(m_globalTransform.GetPosition().x, m_globalTransform.GetPosition().y);
	}
}

void dae::GameObject::SetWorldPosition(float x, float y)
{
	m_globalTransform.SetPosition(x, y, 0.0f);

}

