#include "Subject.h"
#include "GameObject.h"

void dae::Subject::Notify(GameEvent event, GameObject* actor)
{
    // Notify all observers
    for (const auto& observer : m_observers)
        observer->Notify(event, actor);
}

void dae::Subject::AddObserver(dae::Observer* observer)
{
    if (!observer)
        return;

    // Check if the observer list is empty to avoid unnecessary lookups
    if (!m_observers.empty())
    {
        auto it = std::find_if(m_observers.begin(), m_observers.end(),
            [observer](const std::unique_ptr<dae::Observer>& existingObserver)
            {
                return existingObserver.get() == observer;
            });

        if (it != m_observers.end()) // Observer already exists
            return;
    }

    m_observers.emplace_back(std::unique_ptr<dae::Observer>(observer));
}

void dae::Subject::RemoveObserver(dae::Observer* observer)
{
    if (!observer || m_observers.empty()) // Check for null and empty list
        return;

    auto it = std::find_if(m_observers.begin(), m_observers.end(),
        [observer](const std::unique_ptr<dae::Observer>& existingObserver)
        {
            return existingObserver.get() == observer;
        });

    if (it != m_observers.end()) // Remove only if found
    {
        m_observers.erase(it);
    }
}