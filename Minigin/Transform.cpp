#include "Transform.h"

void dae::Transform::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::Transform::SetPosition(const glm::vec3& pos)
{
	m_position = pos;
}
