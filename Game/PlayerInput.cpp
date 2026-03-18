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
    owner->GetTransform().origin = { 0.5f, 1.0f };
}

void DivingInput::Update(float dt) {
    auto* input = owner->GetComponent<InputHandler>();
    auto* grav = owner->GetComponent<GravityComponent>();
    auto& transform = owner->GetTransform();
    if (!input || !grav) return;

    bool isPressed = input->IsActionPressed();

    // --- 1. GESTION DE LA GRAVITÉ DYNAMIQUE ---
    if (isPressed) {
        // En piqué : on alourdit l'oiseau pour gagner de la vitesse
        GravityMultiplier = 5.0f;
    }
    else {
        // En l'air ou en montée : on allège pour favoriser la hauteur
        // Si on monte (vélocité Y négative), on baisse la gravité sous la normale
        if (transform.velocity.y < 0) {
            GravityMultiplier = 1.1f;
        }
        else {
            GravityMultiplier = 1.5f;
        }
    }
    grav->SetGravity({ 0.f, PlayerSettings::GRAVITY * GravityMultiplier });

    // --- 2. DÉTECTION DU SOL ---
    Scene* currentScene = owner->GetScene();
    if (currentScene) {
        float bestSurfaceY = 0.0f;
        sf::Vector2f bestSlopeDir;
        SlopeType bestSlopeType = SlopeType::UP;
        bool foundGroundThisFrame = false;
        float minDistance = 999999.0f;

        float prevX = transform.pos.x - (transform.velocity.x * dt);
        float prevY = transform.pos.y - (transform.velocity.y * dt);

        for (GameObject* obj : currentScene->GetGameObjects()) {
            auto* hill = obj->GetComponent<HillComponent>();
            if (!hill) continue;

            for (const auto& seg : hill->GetSegments()) {
                sf::Vector2f wS = hill->GetWorldPos(seg.start);
                sf::Vector2f wE = hill->GetWorldPos(seg.end);

                float minX = std::min(wS.x, wE.x) - 1.0f;
                float maxX = std::max(wS.x, wE.x) + 1.0f;

                if (transform.pos.x >= minX && transform.pos.x <= maxX) {
                    float t = (transform.pos.x - wS.x) / (wE.x - wS.x);
                    t = std::clamp(t, 0.0f, 1.0f);
                    float surfaceY = wS.y + t * (wE.y - wS.y);

                    float rangeX = wE.x - wS.x;
                    float prevSurfaceY = surfaceY;
                    if (std::abs(rangeX) > 0.0001f) {
                        float prevT = (prevX - wS.x) / rangeX;
                        prevSurfaceY = wS.y + prevT * (wE.y - wS.y);
                    }

                    bool crossedFromAbove = (prevY <= prevSurfaceY + 5.0f) && (transform.pos.y >= surfaceY - 2.0f);
                    float distToSurface = std::abs(transform.pos.y - surfaceY);
                    bool isMovingUp = (transform.pos.y < prevY);

                    if (isMovingUp && transform.pos.y < surfaceY - 2.0f) continue;

                    if (!isGrounded) {
                        if (!crossedFromAbove) continue;
                    }
                    else {
                        if (distToSurface > 30.0f) continue;
                    }

                    if (transform.pos.y <= surfaceY + hill->collisionThickness) {
                        if (transform.velocity.y < -100.0f && transform.pos.y < surfaceY) continue;

                        float dist = std::abs(transform.pos.y - surfaceY);
                        if (dist < minDistance) {
                            minDistance = dist;
                            bestSurfaceY = surfaceY;
                            sf::Vector2f diff = wE - wS;
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

        // --- 3. RÉSOLUTION AU SOL ---
        if (foundGroundThisFrame) {
            transform.pos.y = bestSurfaceY;
            isGrounded = true;

            float speed = std::sqrt(transform.velocity.x * transform.velocity.x + transform.velocity.y * transform.velocity.y);

            // Physique de pente ajustée pour l'inertie
            float accel = 0.0f;
            if (bestSlopeType == SlopeType::DOWN) {
                accel = isPressed ? 1500.0f : 200.0f;
            }
            else {
                // Ici on réduit la perte de vitesse si on ne presse pas (Inertie préservée)
                accel = isPressed ? -2500.0f : -100.0f;
            }

            speed += accel * dt;
            if (speed < 150.f) speed = 150.f;

            // Application de la direction de la pente
            transform.velocity = bestSlopeDir * speed;

            // ejection : Si on arrive en haut d'une pente (pente qui s'adoucit en montant)
            // On aide l'oiseau à décoller
            if (bestSlopeDir.y < -0.05f && !isPressed && speed > 600.0f) {
                transform.velocity.y -= 50.0f * dt;
            }
        }
        else {
            isGrounded = false;

            // --- 4. LOGIQUE DE PLONGÉE ASYMÉTRIQUE ---
            if (isPressed) {
                transform.velocity.y += 500.0f * dt;
                float maxDiveSpeed = 1500.0f;
                if (transform.velocity.y > maxDiveSpeed) transform.velocity.y = maxDiveSpeed;
            }
            else {
                float normalMaxFallSpeed = 700.0f;

                if (transform.velocity.y < 0) {
                    // On allège encore la montée pour aller plus haut
                    transform.velocity.y += std::abs(transform.velocity.y) * 0.8f * dt;
                }
                else if (transform.velocity.y > normalMaxFallSpeed) {
                    // Freinage de la chute (effet planeur)
                    transform.velocity.y -= 400.0f * dt;
                }
            }

            // --- 5. SÉCURITÉ VITESSE MINIMUM ---
            float minForwardSpeed = 250.0f;
            if (transform.velocity.x < minForwardSpeed) {
                transform.velocity.x += 800.0f * dt;
                if (transform.velocity.x > minForwardSpeed) transform.velocity.x = minForwardSpeed;
            }
        }
    }
}