#include "PlayScene.h"
#include "LevelGenerator.h"

void PlayScene::Create()
{
    // background 
    auto background = CreateGameObject({ 0.f, 0.f }, "Background");
    auto backgroundSprite = background->AddComponent<BackgroundScroller>();
    background->SetZOrder(-10);

        //// Première colline (Hill 1)
        //GameObject* h = CreateGameObject({ 0, -20 }, "AutoHill");
        //h->GetTransform().scale = { 8.0f, 8.0f };
        //auto* hill = h->AddComponent<HillComponent>();
        //h->SetZOrder(0);
        //hill->InitFromImage("Assets/chunk1.png", 10);
        //hill->isOneWay = false; // Sol solide


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

    //// level generator
    GameObject* levelManager = CreateGameObject({ 0.f, 0.f }, "LevelManager");
    auto* generator = levelManager->AddComponent<LevelGenerator>();

    // On l'initialise en lui donnant le joueur à suivre
    generator->Init(player);
}