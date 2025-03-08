#pragma once
#include "RenderComponent.h"
namespace dae
{
    class MoveableComponent :
        public RenderComponent
    {
	public:
		void MoveLeft(float elapsedSec);
		void MoveRight(float elapsedSec);
		void MoveUp(float elapsedSec);
		void MoveDown(float elapsedSec);

		MoveableComponent(GameObject& owner, float moveSpeed);
		MoveableComponent(const std::string& filePath, GameObject& owner, float moveSpeed);
		virtual ~MoveableComponent() = default;
		MoveableComponent(const MoveableComponent& other) = delete;
		MoveableComponent(MoveableComponent&& other) = delete;
		MoveableComponent& operator=(const MoveableComponent& other) = delete;
		MoveableComponent& operator=(MoveableComponent&& other) = delete;
	private:
		

		float m_MovementSpeed;
    };
}

