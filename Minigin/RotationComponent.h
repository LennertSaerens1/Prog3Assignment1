#pragma once
#include "RenderComponent.h"

namespace dae
{
	class RotationComponent : public RenderComponent
	{
	public:
		void Update(const float deltaTime) override;

		RotationComponent(GameObject& owner);
		virtual ~RotationComponent() = default;
		RotationComponent(const RotationComponent& other) = delete;
		RotationComponent(RotationComponent&& other) = delete;
		RotationComponent& operator=(const RotationComponent& other) = delete;
		RotationComponent& operator=(RotationComponent&& other) = delete;
	protected:

	};
}

