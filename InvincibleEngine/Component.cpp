#include "Component.h"
#include "GameObject.h"

dae::Component::Component(GameObject& owner)
{
	m_pOwner = &owner;
}
