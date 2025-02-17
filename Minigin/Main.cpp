#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "FpsComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(std::string("background.tga"),*go.get());
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>(std::string("logo.tga"),*go.get());
	go->SetPosition(216, 200);
	scene.Add(go);
	
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextComponent>("Programming 4 Assignment", font, *to.get());
	to->SetPosition(80, 40);
	scene.Add(to);

	auto fps = std::make_shared<dae::GameObject>();
	fps->AddComponent<dae::FpsComponent>(*fps.get());
	fps->SetPosition(0, 0);
	scene.Add(fps);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}