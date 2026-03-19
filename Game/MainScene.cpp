#include <Engine.h>
#include <ModuleManager.h>
#include <SceneModule.h>
#include <WindowModule.h>
#include <GameObject.h>
#include <GravityComponent.h>
#include <SpriteRendererComponent.h>
#include <FSMComponent.h>

#include "BaseScene.h"
#include "PlayerInput.h"
#include "InputHandler.h"
#include "PlayerContext.h"
#include "TestState.h"
#include "PlayScene.h"

#include "Server.h"
#include "Client.h"
#include "World.h"
#include <iostream>

#define DEBUG

int main() {

    // init necessessaire
    Engine* engine = Engine::GetInstance();
    engine->Initialize();

    // Récupération des modules
    SceneModule* sm = engine->GetModuleManager()->GetModule<SceneModule>();
    WindowModule* wm = engine->GetModuleManager()->GetModule<WindowModule>();
    sf::RenderWindow* window = wm->GetRenderWindow();

    // Enregistrement des scènes
    if (sm) {
        sm->RegisterScene<PlayScene>("Play");
        sm->SetScene("Play");


#ifdef DEBUG
        std::cout << "Scene enregistree et lancee." << std::endl;
#endif // DEBUG

    }
    else {
#ifdef DEBUG
        std::cout << "ERREUR : SceneModule introuvable !" << std::endl;
#endif // DEBUG
    }

    // Création du world (menu multijoueur) et du client/serveur
    World* world = new World{};
    Client* client = new Client{world};
    
    engine->Run(client, world);
    return 0;
}