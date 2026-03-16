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

    // --- 2. Détection du Sol (Logique Blindée) ---
    Scene* currentScene = owner->GetScene();
    if (currentScene) {
        float currentX = transform.pos.x;
        float currentY = transform.pos.y;

        // CRUCIAL : On calcule où était le joueur juste avant ce mouvement
        float prevY = currentY - (transform.velocity.y * dt);

        float bestSurfaceY = 999999.0f;
        sf::Vector2f bestSlopeDir;
        SlopeType bestSlopeType = SlopeType::UP;
        bool foundGroundThisFrame = false;

        for (GameObject* obj : currentScene->GetGameObjects()) {
            auto* hill = obj->GetComponent<HillComponent>();
            if (!hill) continue;

            for (const auto& seg : hill->GetSegments()) {
                sf::Vector2f wStart = hill->GetWorldPos(seg.start);
                sf::Vector2f wEnd = hill->GetWorldPos(seg.end);

                // On élargit un tout petit peu la zone X pour éviter les micro-trous entre segments
                float minX = std::min(wStart.x, wEnd.x) - 0.1f;
                float maxX = std::max(wStart.x, wEnd.x) + 0.1f;

                if (currentX >= minX && currentX <= maxX) {
                    float t = (currentX - wStart.x) / (wEnd.x - wStart.x);
                    float surfaceY = wStart.y + t * (wEnd.y - wStart.y);

                    // --- LA LOGIQUE ANTI-TRAVERSÉE ---
                    // Condition A : Le joueur a traversé la ligne (était au dessus, est maintenant en dessous)
                    bool hasCrossed = (prevY <= surfaceY + 2.0f && currentY >= surfaceY - 1.0f);

                    // Condition B : Le joueur est déjà au sol et "glisse" (magnétisme pour les bosses)
                    bool isSliding = (isGrounded && currentY >= surfaceY - 10.0f && currentY <= surfaceY + 20.0f);

                    if (hasCrossed || isSliding) {

                        // Sécurité Envol : Si on a une grosse vitesse vers le haut, on ignore
                        if (transform.velocity.y < -100.0f && currentY < surfaceY) continue;

                        if (surfaceY < bestSurfaceY) {
                            bestSurfaceY = surfaceY;
                            sf::Vector2f diff = wEnd - wStart;
                            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                            bestSlopeDir = diff / (length > 0 ? length : 1.0f);
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
            transform.pos.y = bestSurfaceY;
            isGrounded = true;

            float currentSpeed = std::sqrt(transform.velocity.x * transform.velocity.x + transform.velocity.y * transform.velocity.y);

            if (bestSlopeType == SlopeType::DOWN) {
                float boost = isPressed ? 800.f : 400.f;
                currentSpeed += boost * dt;
            }
            else {
                float friction = isPressed ? 800.f : 1500.f;
                currentSpeed -= friction * dt;
                if (currentSpeed < 150.f) currentSpeed = 150.f;
            }

            transform.velocity = bestSlopeDir * currentSpeed;
        }
        else {
            isGrounded = false;
        }
    }
    wasPressedLastFrame = isPressed;
}