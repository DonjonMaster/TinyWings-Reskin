#include "Engine.h"
#include "ModuleManager.h"
#include "WindowModule.h"

Engine* Engine::instance = { Engine::GetInstance() };
ModuleManager* Engine::moduleManager = { Engine::GetModuleManager() };

void Engine::Initialize()
{

	/*moduleManager->Create();*/
	GetModuleManager()->Create();

}

void Engine::Run()
{
	sf::Clock clock;

	while (!shouldQuit)
	{
		// calcul du dt à chaque début de boucle
		float dt = clock.restart().asSeconds();
		// on empêche un dt trop grand (si on déplace la fenêtre par ex)
		if (dt > 0.05f) dt = 0.05f;

		moduleManager->Update(dt);
		moduleManager->Render();
	}

	moduleManager->Destroy();
}

Engine* Engine::GetInstance()
{
	if (!instance)
		instance = new Engine();
	return instance;
}

ModuleManager* Engine::GetModuleManager()
{
	if (!moduleManager)
		moduleManager = new ModuleManager();
	return moduleManager;
}

void Engine::Quit()
{

	shouldQuit = true;

}