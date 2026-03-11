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
#include <iostream>


class TestComponent : public Component {
public:
    void Render(sf::RenderWindow* window) override {
        // On dessine un énorme carré rouge pour être SUR de le voir
        sf::RectangleShape rect(sf::Vector2f(10, 10));
        rect.setFillColor(sf::Color::Red);
        if (owner) {
            rect.setPosition(owner->GetTransform().pos);
        }
        window->draw(rect);
    }
};

class TestScene : public BaseScene {
public:

    void Create() override
    {
        // ici je peux créer le joueur grace au create player init dans le base scene (on peut toujours mettre les autres elements egalement)
        GameObject* player = CreatePlayer();
        player->AddComponent<TestComponent>();

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
};

int main() {
    // init necessessaire
    Engine* engine = Engine::GetInstance();
    engine->Initialize();

    // application de la scene
    SceneModule* sm = engine->GetModuleManager()->GetModule<SceneModule>();
    if (sm) {
        sm->RegisterScene<TestScene>("Test");
        sm->SetScene("Test");
        std::cout << "Scene enregistree et lancee." << std::endl;
    }
    else {
        std::cout << "ERREUR : SceneModule introuvable !" << std::endl;
    }

    engine->Run();
    return 0;
}