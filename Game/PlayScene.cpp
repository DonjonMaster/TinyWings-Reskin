#include "PlayScene.h"

void PlayScene::Create()
{
    // background 
    auto background = CreateGameObject({ 0.f, 0.f }, "Background");
    auto backgroundSprite = background->AddComponent<BackgroundScroller>();
    background->SetZOrder(-10);

    // creation d'une colline automatique
    GameObject* h = CreateGameObject({ -100, -20 }, "AutoHill");
    h->GetTransform().scale = { 5.0f, 5.0f};
    auto* hill = h->AddComponent<HillComponent>();
    h->SetZOrder(0);

    // le nom correspond à la fonction du .h
    hill->InitFromImage("Assets/chunk1.png", 10);



    // ici je peux créer le joueur grace au create player init dans le base scene (on peut toujours mettre les autres elements egalement)
    GameObject* player = CreatePlayer();
    player->SetZOrder(10);
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