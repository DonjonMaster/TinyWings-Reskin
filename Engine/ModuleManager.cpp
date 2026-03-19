#include "ModuleManager.h"
#include "WindowModule.h"
#include "SceneModule.h"
#include "ResourceModule.h"

void ModuleManager::Create()
{

	// create module ....
	CreateModule<SceneModule>();
	CreateModule<WindowModule>();
	CreateModule<ResourceModule>();

	for (Module* module : modules)
		module->Create();

}

void ModuleManager::Update(float dt)
{

	for (Module* module : modules)
		module->Update(dt);

}

void ModuleManager::Render() {
	auto wm = GetModule<WindowModule>();
	sf::RenderWindow* window = wm->GetRenderWindow();

	for (auto& module : modules) {
		// Le SceneModule doit recevoir le window pour le passer aux GameObjects
		module->Render(window);
	}
}

void ModuleManager::Destroy()
{

	for (Module* module : modules)
		module->Destroy();

}