#pragma once
#include "Component.h"
namespace dae
{

    struct transform
    {
        float matrix[16] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    };

    class GameObject3D
    {
    public:
        transform local;
        int id;
    };
    
    class GameObject3DPtr
    {
    public:
        transform* local;
        int id;
    };

    class TrasheTheCacheComponent : public Component
    {
	private:
		void TrashWithInts(int amount) const;
		void TrashWithGameObject(int amount) const;
		void TrashWithGameObjectPtr(int amount) const;

        bool m_hasPlotted = false;
        mutable std::vector<float> m_timeVector{};
        mutable std::vector<float> m_timeVectorGameObject{};
        mutable std::vector<float> m_timeVectorGameObjectPtr{};

	public:
		void Update(float elapsedSec) override;
		void FixedUpdate(float fixedSec) override;
		void Render(float x, float y) const override;
		void ImGuiRender() override;


		TrasheTheCacheComponent(dae::GameObject& owner);
		virtual ~TrasheTheCacheComponent() = default;
		TrasheTheCacheComponent(const TrasheTheCacheComponent& other) = delete;
		TrasheTheCacheComponent(TrasheTheCacheComponent&& other) = delete;
		TrasheTheCacheComponent& operator=(const TrasheTheCacheComponent& other) = delete;
		TrasheTheCacheComponent& operator=(TrasheTheCacheComponent&& other) = delete;
	
    };
}

