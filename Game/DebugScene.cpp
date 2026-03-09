#include <Engine.h>
#include <ModuleManager.h>
#include <SceneModule.h>
#include <WindowModule.h>
#include <GameObject.h>
#include <GravityComponent.h>

#include "BaseScene.h"

#include <iostream>

// ici j'init un component
class TestComponent : public Component {
public:
    void Render(sf::RenderWindow* window) override {
        // On dessine un énorme carré rouge pour être SUR de le voir
        sf::RectangleShape rect(sf::Vector2f(200, 200));
        rect.setFillColor(sf::Color::Red);
        if (owner) {
            rect.setPosition(owner->GetTransform().pos);
        }
        window->draw(rect);
    }
};

// ici j'applique le component que j'ai init a un gameobject PLAYER
class TestScene : public BaseScene {
public:

    void Create() override
    {
        // ici je peux créer le joueur grace au create player init dans le base scene (on peut toujours mettre les autres elements egalement)
        auto player = CreatePlayer();
        player->AddComponent<TestComponent>();
    }
};

int main() {
    Engine* engine = Engine::GetInstance();
    engine->Initialize();

    // ici je met en place la scene avec tout ce que j'ai besoin d'afficher
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