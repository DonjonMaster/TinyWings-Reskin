#pragma once
#include <SFML/Graphics.hpp>
#include "Module.h"

class WindowModule : public Module
{
public:
	void Create() override;
	void Update(float dt) override;
	void Render() override;

	// Récupère la render window du module
	sf::RenderWindow* GetRenderWindow();

private:
	sf::RenderWindow* window;
};