#include "PlayScene.h"

void PlayScene::Create()
{
    // background 
    auto background = CreateGameObject({ 0.f, 0.f }, "Background");
    auto backgroundSprite = background->AddComponent<BackgroundScroller>();
    background->SetZOrder(-10);

    // creation d'une colline automatique
    GameObject* h = CreateGameObject({ -100, 400 }, "AutoHill");
    auto* hill = h->AddComponent<HillComponent>();
    h->SetZOrder(0);

    // le nom correspond à la fonction du .h
    hill->InitFromImage("Assets/nuage1.png", 10);

    GameObject* h2 = CreateGameObject({ 330, 450 }, "AutoHill2");
    auto* hill2 = h2->AddComponent<HillComponent>();
    hill2->InitFromImage("Assets/nuage2.png", 10);

    GameObject* h3 = CreateGameObject({ 500, 350 }, "AutoHill3");
    auto* hill3 = h3->AddComponent<HillComponent>();
    hill3->InitFromImage("Assets/nuage3.png", 10);

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