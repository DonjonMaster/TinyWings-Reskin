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

    /*World* w = new World{};*/

    // application de la scene
    SceneModule* sm = engine->GetModuleManager()->GetModule<SceneModule>();
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

    //Client c{ w };
    //c.run();

    engine->Run();
    return 0;
}