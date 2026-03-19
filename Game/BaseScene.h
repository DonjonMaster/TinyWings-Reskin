#pragma once
#include <GameObject.h>
#include <Scene.h>
#include <GravityComponent.h>
#include <Component.h>
#include <HillComponent.h>
#include <SpriteRendererComponent.h>
#include <SFML/Graphics.hpp>
#include <InputHandler.h>
#include "PlayerInput.h"    
#include "Background.h"
#include "Server.h"

// dans cette class on peut créer d'autres élément que le create player (en particulier les elements de base present dans la scene !QUI NE SONT PAS DES CLASSES DERIVANT DE SCENE!)
class BaseScene : public Scene
{
private:
	GameObject* player = CreateGameObject({ 500, -800 }, "Player");

protected:

	// permet de créer le joueur dans la scene
	GameObject* CreatePlayer();

	GameObject* CreateGhost();

	// permet de créer un segment des collines
	GameObject* CreateHillSegment(sf::Vector2f pos, sf::Vector2f start, sf::Vector2f end, SlopeType type);

};
