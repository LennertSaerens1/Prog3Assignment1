#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(const float deltaTime)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::FixedUpdate(const float fixedTime)
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate(fixedTime);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void dae::SceneManager::ImGuiRender()
{
	for (const auto& scene : m_scenes)
	{
		scene->ImGuiRender();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
