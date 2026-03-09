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