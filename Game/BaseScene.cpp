#include "BaseScene.h"
#include "InputHandler.h" // Ajoute ces includes
#include "Player.h"       

GameObject* BaseScene::CreatePlayer()
{
	// On commence par enregistrer le joueur dans la scene
	GameObject* player = CreateGameObject({ 640, -120 }, "Player");

	// Configuration du transform
	player->GetTransform().origin = { 0.5f, 1.f };

	// Ajout des composants physiques et logique
	player->AddComponent<GravityComponent>();
	player->AddComponent<InputHandler>(); // Detecte les touches
	player->AddComponent<Player>();       // Pilote la gravite progressive

	return player;
}