#pragma once
#include "TextObject.h"
#include "RenderComponent.h"
#include "Observer.h"
namespace dae
{
    class LivesDisplayObserver :
        public Observer
    {
    public:
        virtual void Notify(GameEvent event, GameObject* actor);
		
		LivesDisplayObserver(GameObject* textToUpdate);
		virtual ~LivesDisplayObserver() = default;
    private:
		RenderComponent* m_livesRenderComp;
    };

    class ScoreDisplayObserver :
        public Observer
    {
    public:
        virtual void Notify(GameEvent event, GameObject* actor);

        ScoreDisplayObserver(GameObject* textToUpdate);
        virtual ~ScoreDisplayObserver() = default;
    private:
        TextComponent* m_scoreTextComp;
    };
}

