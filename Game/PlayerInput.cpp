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

    if (input && grav) {
        // --- Gestion de la Gravité ---
        bool isPressed = input->IsActionPressed();

        // Le bouton augmente juste la gravité (la force qui tire vers le bas)
        if (isPressed) GravityMultiplier += 10.0f * dt;
        else if (GravityMultiplier > 1.0f) GravityMultiplier -= 2.0f * dt;

        GravityMultiplier = std::clamp(GravityMultiplier, 1.0f, 6.0f);
        grav->SetGravity({ 0.f, PlayerSettings::GRAVITY * GravityMultiplier });

        // --- Détection du Sol ---
        Scene* currentScene = owner->GetScene();
        if (currentScene) {
            const auto& allObjects = currentScene->GetGameObjects();

            float hillThickness = 15.0f;

            // CORRECTION ICI : L'aimantation ne dépend QUE de la chute.
            // Le bouton 'isPressed' n'influence plus l'aimantation.
            // Si l'oiseau monte (velocity.y < 0), la marge est de 0. L'inertie l'emportera au sommet !
            float catchMargin = (transform.velocity.y >= 0.0f) ? 35.0f : 0.0f;

            float bestGroundY = 999999.0f;
            sf::Vector2f bestSlopeDir;
            SlopeType bestSlopeType = SlopeType::UP;
            bool foundGround = false;

            float halfWidth = 15.0f;
            float checkXs[3] = { transform.pos.x, transform.pos.x - halfWidth, transform.pos.x + halfWidth };

            for (GameObject* obj : allObjects) {
                auto* hill = obj->GetComponent<HillComponent>();
                if (!hill) continue;

                for (const auto& seg : hill->GetSegments()) {
                    sf::Vector2f wStart = hill->GetWorldPos(seg.start);
                    sf::Vector2f wEnd = hill->GetWorldPos(seg.end);

                    for (int i = 0; i < 3; ++i) {
                        float cx = checkXs[i];

                        if (cx >= wStart.x && cx <= wEnd.x) {
                            float t = (cx - wStart.x) / (wEnd.x - wStart.x);
                            float surfaceY = wStart.y + t * (wEnd.y - wStart.y);

                            float penetration = transform.pos.y - surfaceY;

                            if (penetration >= -catchMargin && penetration <= hillThickness) {
                                if (surfaceY < bestGroundY) {
                                    bestGroundY = surfaceY;
                                    sf::Vector2f diff = wEnd - wStart;
                                    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                                    bestSlopeDir = diff / length;
                                    bestSlopeType = seg.type;
                                    foundGround = true;
                                }
                            }
                        }
                    }
                }
            }

            // --- Résolution de la Collision ---
            bool foundGroundThisFrame = false;

            if (foundGround) {
                // On s'aligne sur la pente
                transform.pos.y = bestGroundY;
                foundGroundThisFrame = true;

                // --- LOGIQUE DE VITESSE (AVEC PÉNALITÉ DE MAUVAIS TIMING) ---
                float currentSpeed = std::sqrt(transform.velocity.x * transform.velocity.x + transform.velocity.y * transform.velocity.y);

                if (bestSlopeType == SlopeType::DOWN) {
                    // En descente : on accélère
                    float boost = 400.f * (GravityMultiplier / 1.5f);
                    currentSpeed += boost * dt;
                }
                else {
                    // En montée :
                    // Si le joueur maintient DIVE en montée = Grosse pénalité de vitesse (comme Tiny Wings)
                    // Si le joueur relâche en montée = Freinage léger normal
                    float friction = isPressed ? 1200.f : 700.f;
                    currentSpeed -= friction * dt;

                    if (currentSpeed < 200.f) currentSpeed = 200.f; // Vitesse minimum
                }

                transform.velocity = bestSlopeDir * currentSpeed;
            }

            isGrounded = foundGroundThisFrame;
        }
        wasPressedLastFrame = isPressed;
    }
}