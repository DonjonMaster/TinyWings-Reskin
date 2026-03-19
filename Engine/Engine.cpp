#include "Engine.h"
#include "ModuleManager.h"
#include "WindowModule.h"
#include "SFML/Graphics.hpp"
#include "../Game/Client.h"
#include "../Game/Server.h"
#include "../Game/World.h"

Engine* Engine::instance = { Engine::GetInstance() };
ModuleManager* Engine::moduleManager = { Engine::GetModuleManager() };

void Engine::Initialize()
{

	/*moduleManager->Create();*/
	GetModuleManager()->Create();

}

void Engine::Run(Client*& client, World*& world)
{
	sf::Clock clock;
	WindowModule* wm = GetModuleManager()->GetModule<WindowModule>();
	sf::RenderWindow* window = wm->GetRenderWindow();

	while (!shouldQuit)
	{
		// Calcul du dt à chaque début de boucle
		float dt = clock.restart().asSeconds();
		// on empêche un dt trop grand (si on déplace la fenêtre par ex)
		if (dt > 0.05f) dt = 0.05f;

		// On choisi quelle fenêtre update
		if (!world->isFinished) {
			world->processEvents(window);
		}
		else {
			wm->Update(dt);
		}
		// Le serveur tourne si on est host
		if (world->hosting) {
			world->server.Run();
		}
		// Dans tout les cas, on est client (il faut envoyer sa position).
		client->run();

		if (!world->isFinished) {
			// MENUS / MULTIJOUEUR
			world->update(dt);
			window->clear();
			world->render(window);
			window->display();
		}
		else {
			// EN JEU
			moduleManager->Update(dt);
			moduleManager->Render();
		}
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