#include "BaseScene.h"
#include "InputHandler.h" // Ajoute ces includes
#include "PlayerInput.h"    

GameObject* BaseScene::CreatePlayer()
{
	// On commence par enregistrer le joueur dans la scene

	// Configuration du transform
	player->GetTransform().origin = { 0.5f, 1.f };
	player->GetTransform().scale = { 0.1f, 0.1f };
	// Ajout des composants physiques et logique
	player->AddComponent<GravityComponent>();
	player->AddComponent<InputHandler>(); // Detecte les touches
	player->AddComponent<SpriteRendererComponent>("Assets/justeUnTest.png");
	player->AddComponent<DivingInput>();       // Pilote la gravite progressive

	return player;
}

// init des hills
GameObject* BaseScene::CreateHillSegment(sf::Vector2f pos, sf::Vector2f start, sf::Vector2f end, SlopeType type)
{
	GameObject* hill = CreateGameObject(pos, "HillSegment");
	auto* comp = hill->AddComponent<HillComponent>();
	comp->Init(start, end, type);
	return hill;
}