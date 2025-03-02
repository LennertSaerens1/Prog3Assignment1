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

    class ImGuiComponent : public Component
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


		ImGuiComponent(dae::GameObject& owner);
		virtual ~ImGuiComponent() = default;
		ImGuiComponent(const ImGuiComponent& other) = delete;
		ImGuiComponent(ImGuiComponent&& other) = delete;
		ImGuiComponent& operator=(const ImGuiComponent& other) = delete;
		ImGuiComponent& operator=(ImGuiComponent&& other) = delete;
	
    };
}

