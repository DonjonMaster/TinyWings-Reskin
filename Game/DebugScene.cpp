#include "Engine.h"
#include "ModuleManager.h"
#include "SceneModule.h"
#include "WindowModule.h"
#include "GameObject.h"
#include <iostream>

// Pour définir les GameObjects
class TestComponent : public Component {
public:
    void Render(sf::RenderWindow* window) override {
        // On dessine un énorme carré rouge pour être SUR de le voir
        sf::RectangleShape rect(sf::Vector2f(200, 200));
        rect.setFillColor(sf::Color::Red);
        rect.setPosition({400, 300});
        window->draw(rect);
    }
};

// Pour initialiser les GameObjects
class TestScene : public Scene {
public:
    void Create() override {
        std::cout << "Scene de test creee !" << std::endl;
        GameObject* obj = CreateGameObject({ 0,0 }, "Tester");
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