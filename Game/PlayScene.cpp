#include "PlayScene.h"
#include "../Engine/SpriteRendererComponent.h"
#include "LevelGenerator.h"
#include "Background.h"

void PlayScene::Create()
{
        // BACKGROUND ICI FREROOOOOOOT
        GameObject* bgObject = CreateGameObject({ 0,0 }, "Background");
        bgObject->AddComponent<Background>("Assets/Backgrounds/image.png");
        bgObject->SetZOrder(-100); 

        //// Premire colline (Hill 1)
        //GameObject* h = CreateGameObject({ 0, -20 }, "AutoHill");
        //h->GetTransform().scale = { 8.0f, 8.0f };
        //auto* hill = h->AddComponent<HillComponent>();
        //h->SetZOrder(0);
        //hill->InitFromImage("Assets/chunk1.png", 10);
        //hill->isOneWay = false; // Sol solide


    // ici je peux crer le joueur grace au create player init dans le base scene (on peut toujours mettre les autres elements egalement)
    GameObject* player = CreatePlayer();
    player->SetZOrder(10);

    // Les fantomes des autres joueurs
    ghost = CreateGhost();
    ghost->SetZOrder(10);

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

    // On l'initialise en lui donnant le joueur � suivre
    generator->Init(player);
}


// un update pour faire le score
void PlayScene::Update(float dt)
{
    BaseScene::Update(dt);

    // je recupere le joueur 
    GameObject* player = nullptr;
    for (auto* obj : GetGameObjects()) {
        if (obj->GetName() == "Player") {
            player = obj;
            break;
        }
    }

    // si il existe 
    if (player)
    {
        // on recup sa pos et augemente le score en cons�quence
        float currentY = player->GetTransform().pos.y;

        // score selon la hauteur
        if (currentY < -300.f) currentScore += score;
        if (currentY < -600.f) currentScore += score * 1.5f;
        if (currentY < -900.f) currentScore += score * 2.f;
        std::cout << "Score : " << currentScore << " posY : " << currentY << std::endl;
    }

    // ghost 
    GameObject* ghost = nullptr;
    for (auto* obj : GetGameObjects()) {
        if (obj->GetName() == "Ghost") {
            ghost = obj;
            break;
        }
    }

    if (ghost)
    {
        // on recup sa pos et augemente le score en cons�quence
        float currentY = ghost->GetTransform().pos.y;

        // score selon la hauteur
        if (currentY < -300.f) currentGhostScore += score;
        if (currentY < -600.f) currentGhostScore += score * 1.5f;
        if (currentY < -900.f) currentGhostScore += score * 2.f;
        std::cout << "Score : " << currentGhostScore << " posY : " << currentY << std::endl;
    }

}