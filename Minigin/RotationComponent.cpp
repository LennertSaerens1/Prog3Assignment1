#include "RotationComponent.h"
#include "cmath"
#include <SDL_stdinc.h>

void dae::RotationComponent::Update(const float deltaTime)
{	
    auto owner = this->GetOwner();
    GameObject* parent = owner->GetParent();

    // Determine middle point
    if (parent != nullptr)
    {
        m_MiddlePos = parent->GetWorldPosition(); // If there is a parent, rotate around its world position
    }

    m_CurrentRotation += (m_angleDegPerSecond * float(M_PI) / 180.0f) * deltaTime; // Convert to radians & scale by deltaTime

    float s = sin(m_CurrentRotation);
    float c = cos(m_CurrentRotation);

    
    glm::vec3 newPosition{};
    if (parent == nullptr)
        newPosition =  glm::vec3(m_MiddlePos.x + s * m_distFromMiddle, m_MiddlePos.y + c * m_distFromMiddle, 0);
    else
    {
        newPosition = glm::vec3(s * m_distFromMiddle, c * m_distFromMiddle, 0);

    }
    owner->SetLocalPosition(newPosition);

    owner->SetPositionDirty();
    owner->UpdateWorldPosition();   

}

dae::RotationComponent::RotationComponent(const std::string& filePath, GameObject& owner)
	:RenderComponent(filePath,owner)
    ,m_CurrentRotation{0}
{
	m_MiddlePos = owner.GetWorldPosition();
}

dae::RotationComponent::RotationComponent(const std::string& filePath, GameObject& owner, float anglePerSec, float distFromMid)
    :RenderComponent(filePath, owner)
    , m_CurrentRotation{ 0 }
{
    m_MiddlePos = owner.GetWorldPosition();
    m_angleDegPerSecond = anglePerSec;
    m_distFromMiddle = distFromMid;
}
