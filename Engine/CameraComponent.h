#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Engine.h"
#include "WindowModule.h"
#include "ModuleManager.h"
#include <algorithm> // Pour std::clamp
#include <cmath>

class CameraComponent : public Component {
private:
    float smoothSpeed = 8.0f;

    // --- VARIABLES POUR LE ZOOM ---
    float currentZoom = 1.0f;
    float zoomSpeed = 5.0f;

    // NOUVELLE VALEUR : Décalée pour s'aligner sur ton nouveau point de spawn
    float baseGroundY = -320.0f;

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

            float currentZoomSpeed = (targetZoom > currentZoom) ? zoomSpeed * 2.0f : zoomSpeed;
            currentZoom += (targetZoom - currentZoom) * currentZoomSpeed * dt;

            // On applique le zoom
            view.setSize(sf::Vector2f(1280.f * currentZoom, 720.f * currentZoom));

            // --- 2. GESTION DE LA POSITION (SUIVI DU JOUEUR) ---
            float offsetX = view.getSize().x / 4.0f;
            sf::Vector2f targetPos(playerPos.x + offsetX, playerPos.y);
            sf::Vector2f currentPos = view.getCenter();

            // Verrouillage strict sur l'axe X (toujours à gauche)
            currentPos.x = targetPos.x;

            // Interpolation douce sur l'axe Y
            currentPos.y += (targetPos.y - currentPos.y) * smoothSpeed * dt;

            // --- 3. SÉCURITÉ ANTI-SORTIE D'ÉCRAN (AXE Y) ---
            float halfHeight = view.getSize().y / 2.0f;
            float margin = 150.0f * currentZoom;
            float topLimit = currentPos.y - halfHeight + margin;
            float bottomLimit = currentPos.y + halfHeight - margin;

            if (playerPos.y < topLimit) {
                currentPos.y = playerPos.y + halfHeight - margin;
            }
            else if (playerPos.y > bottomLimit) {
                currentPos.y = playerPos.y - halfHeight + margin;
            }

            // On applique la nouvelle position
            view.setCenter(currentPos);
            window->setView(view);
        }
    }
};