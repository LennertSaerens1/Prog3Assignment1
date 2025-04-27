#pragma once
#include <vector>
#include <memory>
#include "Observer.h"

namespace dae
{
	class Subject
	{
	public:
		Subject() = default; 
		~Subject() = default;

		void Notify(GameEvent event, GameObject* actor);

		void AddObserver(dae::Observer*);

		void RemoveObserver(dae::Observer*);
	private:
		std::vector<std::unique_ptr<dae::Observer>> m_observers;
	};
}

