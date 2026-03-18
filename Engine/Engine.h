#pragma once
#include "../Game/Client.h"
#include "../Game/World.h"

class ModuleManager;

class Engine
{
public:
	void Initialize();
	void Run(Client*& client, World*& world);
	void Quit();

	// Récupère l'instance du moteur
	static Engine* GetInstance();

	// Récupère l'instance du module manager présent dans le moteur
	static ModuleManager* GetModuleManager();

private:
	static Engine* instance;
	static ModuleManager* moduleManager;
	bool shouldQuit = false;
};