#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Engine.h"
#include "WindowModule.h"
#include <SFML/Graphics.hpp>

class CameraComponent : public Component {
private:
    float smoothSpeed = 8.0f;

public:
    void Update(float dt) override {
        auto windowModule = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>();
        if (windowModule && windowModule->GetRenderWindow()) {
            sf::RenderWindow* window = windowModule->GetRenderWindow();
            sf::View view = window->getView();

            // TAILLE FIXE (Pas de zoom)
            float viewWidth = 1920.f;
            float viewHeight = 1080.f;
            view.setSize({ viewWidth, viewHeight });

            sf::Vector2f playerPos = owner->GetTransform().pos;

            // Suivi horizontal décalé (le joueur est à gauche de l'écran)
            float offsetX = view.getSize().x / 4.0f;

            // On suit le joueur en X et Y
            sf::Vector2f targetPos(playerPos.x + offsetX, playerPos.y);
            sf::Vector2f currentCenter = view.getCenter();

            // Application de la position avec lissage
            currentCenter.x = targetPos.x;
            currentCenter.y += (targetPos.y - currentCenter.y) * smoothSpeed * dt;

            view.setCenter(currentCenter);
            window->setView(view);
        }
    }
};