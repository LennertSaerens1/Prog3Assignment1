#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"

void dae::SceneManager::Update(const float deltaTime)
{
	if (m_HasQueuedLoad) {
		m_QueuedLoadFunction();
		m_HasQueuedLoad = false;
	}
	if (m_activeSceneName.empty())
	{
		// If no active scene is set, update all scenes
		for (const auto& scene : m_scenes)
		{
			scene->Update(deltaTime);
		}
	}
	else
	{
		// Otherwise, only update the active scene
		auto activeScene = GetActiveScene();
		if (activeScene)
		{
			activeScene->Update(deltaTime);
		}
	}
}

void dae::SceneManager::FixedUpdate(const float fixedTime)
{
	if (m_activeSceneName.empty())
	{
		for (const auto& scene : m_scenes)
		{
			scene->FixedUpdate(fixedTime);
		}
	}
	else
	{
		auto activeScene = GetActiveScene();
		if (activeScene)
		{
			activeScene->FixedUpdate(fixedTime);
		}
	}
}

void dae::SceneManager::Render()
{
	if (m_activeSceneName.empty())
	{
		for (const auto& scene : m_scenes)
		{
			scene->Render();
		}
	}
	else
	{
		auto activeScene = GetActiveScene();
		if (activeScene)
		{
			activeScene->Render();
		}
	}
}

void dae::SceneManager::ImGuiRender()
{
	if (m_activeSceneName.empty())
	{
		for (const auto& scene : m_scenes)
		{
			scene->ImGuiRender();
		}
	}
	else
	{
		auto activeScene = GetActiveScene();
		if (activeScene)
		{
			activeScene->ImGuiRender();
		}
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::make_shared<Scene>(name);
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::ClearScenes()
{
	m_scenes.clear();
	m_activeSceneName = "";
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	m_activeSceneName = name;
}

dae::Scene* dae::SceneManager::GetActiveScene() const
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == m_activeSceneName)
			return scene.get();
	}
	return nullptr;
}

bool dae::SceneManager::HasScene(const std::string& name) const
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
			return true;
	}
	return false;
}

void dae::SceneManager::QueueSceneLoad(const std::string& sceneName, const std::function<void()>& loadFunction)
{
	m_QueuedSceneName = sceneName;
	m_QueuedLoadFunction = loadFunction;
	m_HasQueuedLoad = true;
}
