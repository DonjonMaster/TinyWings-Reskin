#pragma once
#include <GameObject.h>
#include <Scene.h>
#include <GravityComponent.h>
#include <Component.h>
#include "HillComponent.h"
#include <SpriteRendererComponent.h>
#include <SFML/Graphics.hpp>

// dans cette class on peut créer d'autres élément que le create player (en particulier les elements de base present dans la scene !QUI NE SONT PAS DES CLASSES DERIVANT DE SCENE!)
class BaseScene : public Scene
{
private:
	GameObject* player = CreateGameObject({ 240, 120 }, "Player");

protected:

	// permet de créer le joueur dans la scene
	GameObject* CreatePlayer();

	// permet de créer un segment des collines
	GameObject* CreateHillSegment(sf::Vector2f pos, sf::Vector2f start, sf::Vector2f end, SlopeType type);

};
