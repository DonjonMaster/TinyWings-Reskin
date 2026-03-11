#include <Engine.h>
#include <ModuleManager.h>
#include <SceneModule.h>
#include <WindowModule.h>
#include <GameObject.h>
#include <GravityComponent.h>
#include <SpriteRendererComponent.h>

#include "BaseScene.h"
#include "PlayerInput.h"
#include "InputHandler.h"
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

        // creation d'une colline automatique
        GameObject* h = CreateGameObject({ -100, 400 }, "AutoHill");
        auto* hill = h->AddComponent<HillComponent>();

        // le nom correspond à la fonction du .h
        hill->InitFromImage("Assets/nuage1.png", 10);


        GameObject* h2 = CreateGameObject({ 330, 450}, "AutoHill2");
        auto* hill2 = h2->AddComponent<HillComponent>();
        hill2->InitFromImage("Assets/nuage2.png", 10);

        GameObject* h3 = CreateGameObject({ 500, -350 }, "AutoHill3");
        auto* hill3 = h3->AddComponent<HillComponent>();
        hill3->InitFromImage("Assets/nuage3.png", 10);
    
    }
};

int main() {
    Engine* engine = Engine::GetInstance();
    engine->Initialize();

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