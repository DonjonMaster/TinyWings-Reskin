#include "PlayScene.h"

void PlayScene::Create()
{
    // background 
    auto background = CreateGameObject({ 0.f, 0.f }, "Background");
    auto backgroundSprite = background->AddComponent<BackgroundScroller>();
    background->SetZOrder(-10);

    // creation d'une colline automatique
    GameObject* h = CreateGameObject({ 0, -20 }, "AutoHill");
    h->GetTransform().scale = { 8.0f, 8.0f};
    auto* hill = h->AddComponent<HillComponent>();
    h->SetZOrder(0);

    // le nom correspond à la fonction du .h
    hill->InitFromImage("Assets/chunk1.png", 10);

    // hill 2
    GameObject* h2 = CreateGameObject({ 9696, 20 }, "AutoHill");
    h2->GetTransform().scale = { 8.0f, 8.0f };
    auto* hill2 = h2->AddComponent<HillComponent>();
    h2->SetZOrder(0);
    hill2->InitFromImage("Assets/chunk1.png", 10);

    // ici je peux créer le joueur grace au create player init dans le base scene (on peut toujours mettre les autres elements egalement)
    GameObject* player = CreatePlayer();
    player->SetZOrder(10);
    player->GetTransform().scale = { 1.0f, 1.0f };
    //player->AddComponent<TestComponent>();

    SpriteRendererComponent* playerSprite = player->GetComponent<SpriteRendererComponent>();

    // FSM du Player
    auto playerFSM = player->AddComponent<FSMComponent<PlayerContext>>();
    auto& machine = playerFSM->fsm;
    auto& ctx = playerFSM->context;

    ctx.player = player;
    ctx.playerSprite = playerSprite;

    TestState* test = machine.CreateState<TestState>();

    machine.Init(test, ctx);
}