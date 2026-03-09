#pragma once
#include <GameObject.h>
#include <Scene.h>
#include <GravityComponent.h>
#include <Component.h>

// dans cette class on peut créer d'autres élément que le create player (en particulier les elements de base present dans la scene !QUI NE SONT PAS DES CLASSES DERIVANT DE SCENE!)
class BaseScene : public Scene
{
protected:

	// permet de créer le joueur dans la scene
	GameObject* CreatePlayer();
};
