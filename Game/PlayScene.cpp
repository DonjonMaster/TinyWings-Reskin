#include "PlayScene.h"
#include "LevelGenerator.h"

void PlayScene::Create()
{
    // background 
    auto background = CreateGameObject({ 0.f, 0.f }, "Background");
    auto backgroundSprite = background->AddComponent<BackgroundScroller>();
    background->SetZOrder(-10);

    // Première colline (Hill 1)
    GameObject* h = CreateGameObject({ 0, -20 }, "AutoHill");
    h->GetTransform().scale = { 8.0f, 8.0f };
    auto* hill = h->AddComponent<HillComponent>();
    h->SetZOrder(0);
    hill->InitFromImage("Assets/chunk1.png", 10);
    hill->isOneWay = false; // Sol solide

    // Première colline (Hill 1)
    GameObject* h3 = CreateGameObject({ 0, -1120 }, "AutoHill");
    h3->GetTransform().scale = { 8.0f, 8.0f };
    auto* hill3 = h3->AddComponent<HillComponent>();
    h3->SetZOrder(0);
    hill3->InitFromImage("Assets/chunk1.png", 10);
    hill3->isOneWay = false; // Sol solide

    // Deuxième colline (Hill 2)
    GameObject* h2 = CreateGameObject({ 9696, -20 }, "AutoHill");
    h2->GetTransform().scale = { 8.0f, 8.0f };
    auto* hill2 = h2->AddComponent<HillComponent>();
    h2->SetZOrder(0);
    hill2->InitFromImage("Assets/chunk1.png", 10);
    hill2->isOneWay = false;

    // Planète (Planet 2)
    GameObject* p2 = CreateGameObject({ 9696, -1000 }, "AutoHill");
    p2->GetTransform().scale = { 3.0f, 3.0f };
    auto* planet2 = p2->AddComponent<HillComponent>();
    p2->SetZOrder(-5); // Un peu en retrait
    planet2->InitFromImage("Assets/planet2.png", 10);
    planet2->isOneWay = true; // Traversable par le dessous

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
    //GameObject* levelManager = CreateGameObject({ 0.f, 0.f }, "LevelManager");
    //auto* generator = levelManager->AddComponent<LevelGenerator>();

    //// On l'initialise en lui donnant le joueur à suivre
    //generator->Init(player);
}