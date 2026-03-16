#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Engine.h"
#include "WindowModule.h"
#include "ModuleManager.h"
#include <algorithm> // Pour std::clamp

class CameraComponent : public Component {
private:
    float smoothSpeed = 5.0f;

    // --- VARIABLES POUR LE ZOOM ---
    float currentZoom = 1.0f;
    float zoomSpeed = 2.0f;
    float baseGroundY = 400.0f;
    float maxZoom = 2.5f;
    float zoomMultiplier = 0.0015f;

public:
    void Update(float dt) override {
        auto windowModule = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>();
        if (windowModule && windowModule->GetRenderWindow()) {
            sf::RenderWindow* window = windowModule->GetRenderWindow();
            sf::View view = window->getView();

            // Position de notre joueur
            sf::Vector2f playerPos = owner->GetTransform().pos;


            // --- 1. GESTION DU ZOOM DYNAMIQUE ---

            float altitude = baseGroundY - playerPos.y;
            float targetZoom = 1.0f;

            if (altitude > 0) {
                targetZoom += altitude * zoomMultiplier;
            }

            targetZoom = std::clamp(targetZoom, 1.0f, maxZoom);
            currentZoom += (targetZoom - currentZoom) * zoomSpeed * dt;

            // On applique le zoom
            view.setSize(sf::Vector2f(1280.f * currentZoom, 720.f * currentZoom));


            // --- 2. GESTION DE LA POSITION (SUIVI DU JOUEUR) ---

            // On décale le centre de la caméra vers la droite du joueur.
            // Le joueur se retrouvera ainsi toujours à 1/4 (25%) de la bordure gauche.
            float offsetX = view.getSize().x / 4.0f;
            sf::Vector2f targetPos(playerPos.x + offsetX, playerPos.y);
            sf::Vector2f currentPos = view.getCenter();

            // CORRECTION ICI : Verrouillage strict sur l'axe X (pas de "lag" de caméra)
            currentPos.x = targetPos.x;

            // Interpolation douce uniquement sur l'axe Y (pour les sauts et chutes)
            currentPos.y += (targetPos.y - currentPos.y) * smoothSpeed * dt;

            // Optionnel : limite basse pour la caméra (empêche de voir sous le sol)
            // if (currentPos.y > baseGroundY) currentPos.y = baseGroundY;

            // On applique la nouvelle position
            view.setCenter(currentPos);
            window->setView(view);
        }
    }
};