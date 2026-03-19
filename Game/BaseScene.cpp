#include "BaseScene.h"
#include "CameraComponent.h"
#include "TimerComponent.h"

GameObject* BaseScene::CreatePlayer()
{
	// On commence par enregistrer le joueur dans la scene

	// Configuration du transform
	player->GetTransform().origin = { 0.5f, 1.f };
	player->GetTransform().scale = { 1.f, 1.f };
	// Ajout des composants physiques et logique
	player->AddComponent<GravityComponent>();
	player->AddComponent<InputHandler>(); // Detecte les touches
	player->AddComponent<SpriteRendererComponent>("Assets/hamster_qui_roule.png");
	player->AddComponent<DivingInput>();       // Pilote la gravite progressive
	player->AddComponent<CameraComponent>();	// Camera
	player->AddComponent<TimerComponent>();		// Timer
	player->SetZOrder(800);

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


GameObject* BaseScene::CreateGhost()
{
	ghost = CreateGameObject({ 240, 120 }, "Ghost");

	ghost->GetTransform().origin = player->GetTransform().origin;
	ghost->GetTransform().scale = player->GetTransform().scale;
	ghost->GetTransform().pos = player->GetTransform().pos;

	ghost->AddComponent<SpriteRendererComponent>("Assets/hamster_qui_roule2.png");
	player->SetZOrder(799);
	return ghost;
}