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

    // --- NOUVELLES VARIABLES POUR LE ZOOM ---
    float currentZoom = 1.0f;      // Le niveau de zoom actuel
    float zoomSpeed = 2.0f;        // Vitesse de transition du zoom (plus lent que la caméra pour un effet dramatique)
    float baseGroundY = 400.0f;    // L'altitude "de base" où le zoom est normal (1.0x)
    float maxZoom = 2.5f;          // Limite maximum du dézoom pour ne pas voir tout le niveau
    float zoomMultiplier = 0.0015f;// Sensibilité du dézoom par rapport à l'altitude

public:
    void Update(float dt) override {
        auto windowModule = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>();
        if (windowModule && windowModule->GetRenderWindow()) {
            sf::RenderWindow* window = windowModule->GetRenderWindow();
            sf::View view = window->getView();

            // Position de notre joueur
            sf::Vector2f playerPos = owner->GetTransform().pos;

            // ----------------------------------------------------
            // 1. GESTION DU ZOOM DYNAMIQUE
            // ----------------------------------------------------
            // Calcul de l'altitude : "baseGroundY - playerPos.y"
            // Plus on monte (playerPos.y diminue), plus l'altitude est grande.
            float altitude = baseGroundY - playerPos.y;
            float targetZoom = 1.0f;

            if (altitude > 0) {
                targetZoom += altitude * zoomMultiplier;
            }

            // On empêche le dézoom de dépasser la limite max
            targetZoom = std::clamp(targetZoom, 1.0f, maxZoom);

            // Interpolation (LERP) du zoom pour que ce soit fluide
            currentZoom += (targetZoom - currentZoom) * zoomSpeed * dt;

            // On applique le zoom en changeant la taille de la vue
            // (1280x720 est la taille de base définie dans ton WindowModule)
            view.setSize(sf::Vector2f(1280.f * currentZoom, 720.f * currentZoom));

            // ----------------------------------------------------
            // 2. GESTION DE LA POSITION (SUIVI DU JOUEUR)
            // ----------------------------------------------------
            // On ajuste le décalage pour que le joueur reste au quart gauche, 
            // même quand on dézoome (la taille de la vue change !)
            float offsetX = view.getSize().x / 4.0f;

            sf::Vector2f targetPos(playerPos.x + offsetX, playerPos.y);
            sf::Vector2f currentPos = view.getCenter();

            currentPos.x += (targetPos.x - currentPos.x) * smoothSpeed * dt;
            currentPos.y += (targetPos.y - currentPos.y) * smoothSpeed * dt;

            // Optionnel : limite basse pour la caméra (empêche de voir sous le sol)
            // Si tu as un sol très bas, tu peux décommenter la ligne suivante :
            // if (currentPos.y > baseGroundY) currentPos.y = baseGroundY;

            // On applique la nouvelle position
            view.setCenter(currentPos);
            window->setView(view);
        }
    }
};