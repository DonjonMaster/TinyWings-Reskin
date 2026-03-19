#include "PlayScene.h"
#include "../Engine/SpriteRendererComponent.h"
#include "Engine.h"
#include "LevelGenerator.h"
#include "Background.h"
#include "TimerRenderer.h"
#include "TimerComponent.h"
#include <ScoreRenderer.h>

void PlayScene::Create()
{
        // BACKGROUND ICI FREROOOOOOOT
        GameObject* bgObject = CreateGameObject({ 0,0 }, "Background");
        bgObject->AddComponent<Background>("Assets/Backgrounds/tata.jpg");
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

    // On crée un objet vide qui servira de support à l'interface
    auto hud = CreateGameObject({ 0.f, 0.f }, "HUD");
    hud->AddComponent<ScoreRenderer>();

    // On s'assure qu'il est dessiné au-dessus de tout le reste (Z-Order élevé)
    hud->SetZOrder(1000);

    //// level generator
    GameObject* levelManager = CreateGameObject({ 0.f, 0.f }, "LevelManager");
    auto* generator = levelManager->AddComponent<LevelGenerator>();

    // On l'initialise en lui donnant le joueur � suivre
    generator->Init(player);


    GameObject* uiManager = CreateGameObject({ 0.f, 0.f }, "UIManager");
    uiManager->AddComponent<TimerRenderer>();
    uiManager->SetZOrder(100);
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

        auto* timers = player->GetComponent<TimerComponent>();

        // C'est ici qu'on utilise le booléen !
        if (timers && timers->isGameOver)
        {
            std::cout << "Fermeture du jeu suite au Game Over..." << std::endl;


            Engine::GetInstance()->Quit();
        }


        // on recup sa pos et augemente le score en cons�quence
        float currentY = player->GetTransform().pos.y;

        // score selon la hauteur
        if (currentY < -850.f) currentScore += score;
        if (currentY < -1200.f) currentScore += score * 1.5f;
        if (currentY < -2500.f) currentScore += score * 2.f;
        if (currentY < -3500.f) currentScore += score * 2.5f;
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
        if (currentY < -850.f) currentGhostScore += score;
        if (currentY < -1500.f) currentGhostScore += score * 1.5f;
        if (currentY < -2500.f) currentGhostScore += score * 2.f;
        if (currentY < -3500.f) currentGhostScore += score * 2.5f;
        std::cout << "Score : " << currentGhostScore << " posY : " << currentY << std::endl;
    }

}