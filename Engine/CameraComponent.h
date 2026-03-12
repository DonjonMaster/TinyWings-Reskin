#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Engine.h"
#include "WindowModule.h"
#include "ModuleManager.h"

class CameraComponent : public Component {
private:
    // Plus la valeur est basse, plus la caméra est "élastique" et lente à rattraper le joueur.
    // À 5.0f, c'est généralement un bon équilibre pour un jeu rapide.
    float smoothSpeed = 5.0f;

public:
    void Update(float dt) override {
        auto windowModule = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>();

        // On remplace GetWindow() par GetRenderWindow() ici :
        if (windowModule && windowModule->GetRenderWindow()) {
            sf::RenderWindow* window = windowModule->GetRenderWindow();
            sf::View view = window->getView();

            // Position de notre joueur
            sf::Vector2f playerPos = owner->GetTransform().pos;

            // Décalage pour placer le joueur sur la gauche de l'écran (le quart gauche)
            float offsetX = view.getSize().x / 4.0f;

            // C'est ici que la caméra DOIT aller idéalement
            sf::Vector2f targetPos(playerPos.x + offsetX, playerPos.y);

            // Position ACTUELLE de la caméra
            sf::Vector2f currentPos = view.getCenter();

            // LERP : On rapproche doucement la position actuelle vers la cible
            currentPos.x += (targetPos.x - currentPos.x) * smoothSpeed * dt;
            currentPos.y += (targetPos.y - currentPos.y) * smoothSpeed * dt;

            // On applique la nouvelle position et on met à jour la fenêtre
            view.setCenter(currentPos);
            window->setView(view);
        }
    }
};