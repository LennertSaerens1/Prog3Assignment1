#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <functional>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(const float deltaTime);
		void FixedUpdate(const float fixedTime);
		void Render();
		void ImGuiRender();

		void ClearScenes();
		void SetActiveScene(const std::string& name);
		Scene* GetActiveScene() const;
		bool HasScene(const std::string& name) const;

		void QueueSceneLoad(const std::string& sceneName, const std::function<void()>& loadFunction);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::string m_activeSceneName;

		std::string m_QueuedSceneName;
		std::function<void()> m_QueuedLoadFunction;
		bool m_HasQueuedLoad = false;
	};
}