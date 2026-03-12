#include "PlayerInput.h"
#include "GameObject.h"  
#include "Constants.h"
#include "GravityComponent.h"
#include "InputHandler.h"
#include "HillComponent.h"   
#include <Scene.h>           
#include <iostream>
#include <algorithm>
#include <vector>          

void DivingInput::Create() {
    GravityMultiplier = 1.0f;
    owner->GetTransform().velocity = PlayerSettings::START_VELOCITY;

    // CRUCIAL : Place le point de collision aux PIEDS du joueur (et non au centre)
    // C'est ça qui empêche ton bonhomme d'être à moitié dans le sol !
    owner->GetTransform().origin = { 0.5f, 1.0f };
}

void DivingInput::Update(float dt) {
    auto* input = owner->GetComponent<InputHandler>();
    auto* grav = owner->GetComponent<GravityComponent>();
    auto& transform = owner->GetTransform();

    if (input && grav) {
        // --- Gestion de la Gravité ---
        bool isPressed = input->IsActionPressed();
        if (isPressed) GravityMultiplier += 10.0f * dt;
        else if (GravityMultiplier > 1.0f) GravityMultiplier -= 1.5f * dt;
        GravityMultiplier = std::clamp(GravityMultiplier, 1.0f, 6.0f);
        grav->SetGravity({ 0.f, PlayerSettings::GRAVITY * GravityMultiplier });

        // --- Détection du Sol (Méthode des 3 Capteurs type "Sonic") ---
        Scene* currentScene = owner->GetScene();
        if (currentScene) {
            const auto& allObjects = currentScene->GetGameObjects();
            float detectionThreshold = 150.0f;

            float centerGroundY = 999999.0f;
            float bestGroundY = 999999.0f;
            sf::Vector2f slopeDir;
            SlopeType slopeType = SlopeType::UP;
            bool foundGround = false;

            // On prend un peu moins que la vraie largeur (20 au lieu de 25) 
            // pour que les bords soient plus tolérants sur les petites bosses
            float halfWidth = 20.0f;

            for (GameObject* obj : allObjects) {
                auto* hill = obj->GetComponent<HillComponent>();
                if (!hill) continue;

                for (const auto& seg : hill->GetSegments()) {
                    sf::Vector2f wStart = hill->GetWorldPos(seg.start);
                    sf::Vector2f wEnd = hill->GetWorldPos(seg.end);

                    // 1. Capteur CENTRAL (Sert aussi à calculer l'inclinaison pour la vitesse)
                    if (transform.pos.x >= wStart.x && transform.pos.x <= wEnd.x) {
                        float t = (transform.pos.x - wStart.x) / (wEnd.x - wStart.x);
                        centerGroundY = wStart.y + t * (wEnd.y - wStart.y);

                        sf::Vector2f diff = wEnd - wStart;
                        float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                        slopeDir = diff / length;
                        slopeType = seg.type;
                        foundGround = true;
                    }

                    // 2. Capteur PIED GAUCHE
                    float leftX = transform.pos.x - halfWidth;
                    if (leftX >= wStart.x && leftX <= wEnd.x) {
                        float t = (leftX - wStart.x) / (wEnd.x - wStart.x);
                        float y = wStart.y + t * (wEnd.y - wStart.y);
                        if (y < bestGroundY) bestGroundY = y;
                    }

                    // 3. Capteur PIED DROIT
                    float rightX = transform.pos.x + halfWidth;
                    if (rightX >= wStart.x && rightX <= wEnd.x) {
                        float t = (rightX - wStart.x) / (wEnd.x - wStart.x);
                        float y = wStart.y + t * (wEnd.y - wStart.y);
                        if (y < bestGroundY) bestGroundY = y;
                    }
                }
            }

            // --- Résolution de la Collision ---
            if (foundGround) {
                // On garde le point physique le plus haut entre le centre et les pieds
                bestGroundY = std::min(bestGroundY, centerGroundY);

                float penetration = transform.pos.y - bestGroundY;

                // Si le joueur est en train de toucher ou de s'enfoncer dans le sol
                if (penetration >= 0.0f && penetration < detectionThreshold) {

                    // On le pose pile sur le point le plus haut !
                    transform.pos.y = bestGroundY;

                    // --- LOGIQUE DE VITESSE ---
                    float currentSpeed = std::sqrt(transform.velocity.x * transform.velocity.x + transform.velocity.y * transform.velocity.y);

                    if (slopeType == SlopeType::DOWN) {
                        float boost = 400.f * (GravityMultiplier / 2.0f);
                        currentSpeed += boost * dt;
                    }
                    else {
                        currentSpeed -= 1000.f * dt;
                        if (currentSpeed < 100.f) currentSpeed = 100.f;
                    }

                    transform.velocity = slopeDir * currentSpeed;
                }
            }
        }
        wasPressedLastFrame = isPressed;
    }
}