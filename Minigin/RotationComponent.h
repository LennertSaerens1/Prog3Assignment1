#pragma once
#include "RenderComponent.h"

namespace dae
{
	class RotationComponent : public RenderComponent
	{
	public:
		void Update(const float deltaTime) override;

		RotationComponent(const std::string& filePath, GameObject& owner);
		RotationComponent(const std::string& filePath, GameObject& owner, float anglePerSec, float distFromMid);
		virtual ~RotationComponent() = default;
		RotationComponent(const RotationComponent& other) = delete;
		RotationComponent(RotationComponent&& other) = delete;
		RotationComponent& operator=(const RotationComponent& other) = delete;
		RotationComponent& operator=(RotationComponent&& other) = delete;
	protected:

		glm::uvec3 m_MiddlePos;
		float m_CurrentRotation;
		float m_angleDegPerSecond = 360.0f; // Rotate 3 degrees per second

		float m_distFromMiddle{ 30 };
	};
}

