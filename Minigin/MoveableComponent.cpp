#include "MoveableComponent.h"

dae::MoveableComponent::MoveableComponent(GameObject& owner, float moveSpeed)
	:RenderComponent(owner)
{
	m_MovementSpeed = moveSpeed;
}

dae::MoveableComponent::MoveableComponent(const std::string& filePath, GameObject& owner, float moveSpeed)
	:RenderComponent(filePath,owner)
{
	m_MovementSpeed = moveSpeed;
}

void dae::MoveableComponent::MoveLeft(float elapsedSec)
{
	auto owner = this->GetOwner();
	owner->AddWorldOffset(glm::vec3(-m_MovementSpeed * elapsedSec,0,0));
}

void dae::MoveableComponent::MoveRight(float elapsedSec)
{
	auto owner = this->GetOwner();
	owner->AddWorldOffset(glm::vec3(m_MovementSpeed * elapsedSec, 0, 0));
}

void dae::MoveableComponent::MoveUp( float elapsedSec)
{
	auto owner = this->GetOwner();
	owner->AddWorldOffset(glm::vec3(0, -m_MovementSpeed * elapsedSec, 0));
}

void dae::MoveableComponent::MoveDown(float elapsedSec)
{
	auto owner = this->GetOwner();
	owner->AddWorldOffset(glm::vec3(0, m_MovementSpeed * elapsedSec, 0));
}