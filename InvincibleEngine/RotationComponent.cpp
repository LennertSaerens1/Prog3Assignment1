#include "RotationComponent.h"
#include "cmath"
#include <SDL_stdinc.h>

void dae::RotationComponent::Update(const float deltaTime)
{	
    auto owner = this->GetOwner();

    m_CurrentRotation += (m_angleDegPerSecond * float(M_PI) / 180.0f) * deltaTime; // Convert to radians & scale by deltaTime
    //Check if angle is greater than 2 pi or less than -2pi and change it accordingly with +2pi or -2pi


    float s = sin(m_CurrentRotation);
    float c = cos(m_CurrentRotation);

    glm::vec3 newPosition{};
        newPosition = glm::vec3(c * m_distFromMiddle, s * m_distFromMiddle, 0);

    owner->SetLocalPosition(newPosition);
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
