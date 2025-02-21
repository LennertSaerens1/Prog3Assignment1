#include "RotationComponent.h"

void dae::RotationComponent::Update(const float deltaTime)
{
	auto owner = this->GetOwner();
	
}

dae::RotationComponent::RotationComponent(GameObject& owner)
	:RenderComponent(owner)
{
}
