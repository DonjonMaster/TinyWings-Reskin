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
#include <iostream>

#define DEBUG

//class TestComponent : public Component {
//public:
//    void Render(sf::RenderWindow* window) override {
//        // On dessine un énorme carré rouge pour être SUR de le voir
//        sf::RectangleShape rect(sf::Vector2f(10, 10));
//        rect.setFillColor(sf::Color::Red);
//        if (owner) {
//            rect.setPosition(owner->GetTransform().pos);
//        }
//        window->draw(rect);
//    }
//};

int main() {
    // init necessessaire
    Engine* engine = Engine::GetInstance();
    engine->Initialize();

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

    engine->Run();
    return 0;
}