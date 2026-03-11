#include "BaseScene.h"
#include "InputHandler.h" // Ajoute ces includes
#include "PlayerInput.h"    

GameObject* BaseScene::CreatePlayer()
{
	// On commence par enregistrer le joueur dans la scene
	//GameObject* player = CreateGameObject({ 240, 120 }, "Player");

	// Configuration du transform
	player->GetTransform().origin = { 0.5f, 1.f };
	player->GetTransform().scale = { 2.f, 2.f };
	// Ajout des composants physiques et logique
	player->AddComponent<GravityComponent>();
	player->AddComponent<InputHandler>(); // Detecte les touches
	player->AddComponent<SpriteRendererComponent>("Assets/TestPourAnim.png");
	player->AddComponent<DivingInput>();       // Pilote la gravite progressive

	return player;
}