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

        // --- COLLINE 1 ---
        // Montée rouge
        CreateHillSegment({ 300, 500 }, { 0, 100 }, { 200, 0 }, SlopeType::UP);
        // Descente bleue
        CreateHillSegment({ 300, 500 }, { 200, 0 }, { 400, 100 }, SlopeType::DOWN);

        // --- COLLINE 2 ---
        // Montée rouge
        CreateHillSegment({ 700, 500 }, { 0, 100 }, { 250, 0 }, SlopeType::UP);
        // Descente bleue
        CreateHillSegment({ 700, 500 }, { 250, 0 }, { 500, 100 }, SlopeType::DOWN);
    
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