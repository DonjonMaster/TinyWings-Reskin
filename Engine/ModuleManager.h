#pragma once
#include <iostream>
#include <vector>
#include "Module.h"

class ModuleManager
{
public:
	void Create();
	void Update(float dt);
	void Render();
	void Destroy();

	// Créer un module dans le module manager
	template<typename ModuleType>
	ModuleType* CreateModule();

	// Récupère un module dans le module manager
	// /!\ - Le résultat peut être nullptr, à bien vérifier car cela peut causer un crash
	template<typename ModuleType>
	ModuleType* GetModule();

private:
	std::vector<Module*> modules;
};

#include "ModuleManager.inl"