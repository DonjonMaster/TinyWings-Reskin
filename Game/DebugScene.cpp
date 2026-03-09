#include "Engine.h"
#include "ModuleManager.h"
#include "SceneModule.h"
#include "WindowModule.h"
#include "GameObject.h"
#include "GravityComponent.h"
#include "Player.h"
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

class TestScene : public Scene {
public:
    void Create() override {
        std::cout << "Scene de test creee !" << std::endl;
        GameObject* obj = CreateGameObject({ 0,0 }, "Tester");
        // 1. Détecte les touches (Engine)
        obj->AddComponent<InputHandler>();

        // 2. Gère la chute (Engine)
        obj->AddComponent<GravityComponent>();

        // 3. Pilote le tout (Game)
        obj->AddComponent<Player>();

        // 4. Le visuel (Game)
        obj->AddComponent<TestComponent>();
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