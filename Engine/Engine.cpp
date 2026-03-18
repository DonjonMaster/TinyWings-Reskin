#include "Engine.h"
#include "ModuleManager.h"
#include "WindowModule.h"
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
		float dt = clock.restart().asSeconds();
		if (dt > 0.05f) dt = 0.05f; // Cap dt to prevent large jumps (e.g. window drag)

		// Process window events
		if (!world->isFinished) {
			world->processEvents(window);
		}
		else {
			wm->Update(dt);
		}
		// BEGIN OF FRAME: receive all pending packets
		if (world->hosting) {
			world->server.DrainReceive();
		}
		client->Run();

		if (!world->isFinished) {
			// MENU / LOBBY
			world->update(dt);
			window->clear();
			world->render(window);
			window->display();
		}
		else {
			// IN-GAME
			moduleManager->Update(dt);
			moduleManager->Render();
		}

		// END OF FRAME: relay accumulated state to all clients
		if (world->hosting) {
			world->server.FlushSends();
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