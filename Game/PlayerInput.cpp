#include "PlayerInput.h"
#include "GameObject.h"  
#include "Constants.h"
#include "GravityComponent.h"
#include "InputHandler.h"
#include "HillComponent.h"   
#include <Scene.h>            
#include <iostream>
#include <algorithm>
#include <cmath>

void DivingInput::Create() {
    GravityMultiplier = 1.0f;
    owner->GetTransform().velocity = PlayerSettings::START_VELOCITY;

    // CRUCIAL : Point de collision aux pieds
    owner->GetTransform().origin = { 0.5f, 1.0f };
}

void DivingInput::Update(float dt) {
    auto* input = owner->GetComponent<InputHandler>();
    auto* grav = owner->GetComponent<GravityComponent>();
    auto& transform = owner->GetTransform();

    if (!input || !grav) return;

    // --- 1. Physique de base ---
    bool isPressed = input->IsActionPressed();
    if (isPressed) GravityMultiplier += 10.0f * dt;
    else if (GravityMultiplier > 1.0f) GravityMultiplier -= 2.0f * dt;
    GravityMultiplier = std::clamp(GravityMultiplier, 1.0f, 6.0f);

    grav->SetGravity({ 0.f, PlayerSettings::GRAVITY * GravityMultiplier });

    // --- 2. Détection du Sol ---
    Scene* currentScene = owner->GetScene();
    if (currentScene) {
        float currentX = transform.pos.x;
        float currentY = transform.pos.y;

        float bestSurfaceY = 999999.0f;
        sf::Vector2f bestSlopeDir;
        SlopeType bestSlopeType = SlopeType::UP;
        bool foundGroundThisFrame = false;

        // Paramètres de capture
        float verticalFall = transform.velocity.y * dt;
        float snapBottom = std::max(10.0f, verticalFall + 5.0f);
        float snapTop = 5.0f; // On réduit le snapTop pour permettre le décollage

        for (GameObject* obj : currentScene->GetGameObjects()) {
            auto* hill = obj->GetComponent<HillComponent>();
            if (!hill) continue;

            for (const auto& seg : hill->GetSegments()) {
                sf::Vector2f wStart = hill->GetWorldPos(seg.start);
                sf::Vector2f wEnd = hill->GetWorldPos(seg.end);

                float minX = std::min(wStart.x, wEnd.x);
                float maxX = std::max(wStart.x, wEnd.x);

                if (currentX >= minX && currentX <= maxX) {
                    float t = (currentX - wStart.x) / (wEnd.x - wStart.x);
                    float surfaceY = wStart.y + t * (wEnd.y - wStart.y);

                    // --- CONDITION DE COLLISION AMÉLIORÉE ---
                    // On ne collisionne que si :
                    // 1. On est dans la zone (snap)
                    // 2. ET on est en train de descendre (velocity.y > 0) 
                    //    OU on était déjà au sol (isGrounded)
                    if (currentY >= surfaceY - snapTop && currentY <= surfaceY + snapBottom) {

                        // SI l'oiseau va vers le haut (velocity.y < -50) et qu'il dépasse la surface
                        // ALORS on le laisse s'envoler (on ignore la collision)
                        if (transform.velocity.y < -50.0f && currentY < surfaceY) continue;

                        if (surfaceY < bestSurfaceY) {
                            bestSurfaceY = surfaceY;
                            sf::Vector2f diff = wEnd - wStart;
                            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                            bestSlopeDir = diff / length;
                            if (bestSlopeDir.x < 0) bestSlopeDir = -bestSlopeDir;
                            bestSlopeType = seg.type;
                            foundGroundThisFrame = true;
                        }
                    }
                }
            }
        }

        // --- 3. Résolution ---
        if (foundGroundThisFrame) {
            // On ne plaque au sol que si on n'est pas en train de "jumper"
            transform.pos.y = bestSurfaceY;
            isGrounded = true;

            float currentSpeed = std::sqrt(transform.velocity.x * transform.velocity.x + transform.velocity.y * transform.velocity.y);

            if (bestSlopeType == SlopeType::DOWN) {
                float boost = isPressed ? 800.f : 400.f;
                currentSpeed += boost * dt;
            }
            else {
                // En montée, si on n'appuie pas, on perd beaucoup de vitesse
                float friction = isPressed ? 800.f : 1500.f;
                currentSpeed -= friction * dt;

                // Si la vitesse est trop faible en montée, on risque de rester bloqué
                if (currentSpeed < 150.f) currentSpeed = 150.f;
            }

            transform.velocity = bestSlopeDir * currentSpeed;
        }
        else {
            // LIBERTÉ : L'oiseau est en l'air
            isGrounded = false;
        }
    }
    wasPressedLastFrame = isPressed;
}