#include "BaseScene.h"

GameObject* BaseScene::CreatePlayer()
{
	// on commence par enregistrer le joueur dans la scene du gameobject
	GameObject* player = CreateGameObject({ 640, -120 }, "Player");
	// puis vient les inits
	player->GetTransform().origin = { 0.5f, 1.f };
	player->AddComponent<GravityComponent>();
	return player;
}
