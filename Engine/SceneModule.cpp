#include "SceneModule.h"

void SceneModule::Create()
{

	windowModule = Engine::GetModuleManager()->GetModule<WindowModule>();

}

void SceneModule::Update(float dt)
{
	for (Scene* scene : sceneStack)
		scene->Update(dt);

}

void SceneModule::Render()
{
	// 1. On récupère le WindowModule
	WindowModule* wm = Engine::GetModuleManager()->GetModule<WindowModule>();

	if (wm && wm->GetRenderWindow()) {
		sf::RenderWindow* window = wm->GetRenderWindow();

		// 2. On demande à chaque scène de se dessiner sur cette fenêtre
		for (Scene* scene : sceneStack) {
			scene->Render(window);
		}
	}

	//sf::RenderWindow* window = windowModule->GetRenderWindow();

	//for (Scene* scene : sceneStack)
	//	scene->Render(window);

}

void SceneModule::SetScene(std::string name)
{

	if (!sceneConstructorsMap[name])
		return;

	sceneStack.clear();
	Scene* scene = sceneConstructorsMap[name]();

	scene->SetName(name); // applique un nom a la scene
	scene->Create();
	sceneStack.push_back(scene);

}

Scene* SceneModule::GetCurrentScene()
{
	if (sceneStack.empty()) return nullptr;
	return sceneStack.back();
}

Scene* SceneModule::GetSceneWithName(std::string name)
{
	for (Scene* scene : sceneStack)
	{
		if (scene->GetName() == name)
		{
			return scene;
		}
	}
	return nullptr;
}

void SceneModule::PushScene(std::string name)
{

	if (!sceneConstructorsMap[name])
		return;

	Scene* scene = sceneConstructorsMap[name]();
	sceneStack.push_back(scene);

}