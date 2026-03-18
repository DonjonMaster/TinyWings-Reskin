#pragma once

class ModuleManager;
class Client;
class World;

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