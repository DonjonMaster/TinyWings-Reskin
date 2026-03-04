#include "ModuleManager.h"
#include "WindowModule.h"
#include "SceneModule.h"

void ModuleManager::Create()
{

	// create module ....
	CreateModule<SceneModule>();
	CreateModule<WindowModule>();

	for (Module* module : modules)
		module->Create();

}

void ModuleManager::Update(float dt)
{

	for (Module* module : modules)
		module->Update(dt);

}

void ModuleManager::Render()
{

	for (Module* module : modules)
		module->Render();

}

void ModuleManager::Destroy()
{

	for (Module* module : modules)
		module->Destroy();

}