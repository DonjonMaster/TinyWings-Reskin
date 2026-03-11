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
}

void DivingInput::Update(float dt) {
    auto* input = owner->GetComponent<InputHandler>();
    auto* grav = owner->GetComponent<GravityComponent>();
    auto& transform = owner->GetTransform();

    if (input && grav) {
        // --- Gestion de la Gravité (ton code existant) ---
        bool isPressed = input->IsActionPressed();
        if (isPressed) GravityMultiplier += 9.0f * dt;
        else if (GravityMultiplier > 1.0f) GravityMultiplier -= 1.5f * dt;
        GravityMultiplier = std::clamp(GravityMultiplier, 1.0f, 6.0f);
        grav->SetGravity({ 0.f, PlayerSettings::GRAVITY * GravityMultiplier });

        // --- Détection du Sol et de la Pente ---
        Scene* currentScene = owner->GetScene();
        if (currentScene) {
            const auto& allObjects = currentScene->GetGameObjects();
            float detectionThreshold = 150.0f;
            bool foundGround = false;

            for (GameObject* obj : allObjects) {
                auto* hill = obj->GetComponent<HillComponent>();
                if (!hill) continue;

                for (const auto& seg : hill->GetSegments()) {
                    sf::Vector2f wStart = hill->GetWorldPos(seg.start);
                    sf::Vector2f wEnd = hill->GetWorldPos(seg.end);

                    // Si le joueur est horizontalement dans ce segment
                    if (transform.pos.x >= wStart.x && transform.pos.x <= wEnd.x) {
                        float t = (transform.pos.x - wStart.x) / (wEnd.x - wStart.x);
                        float groundY = wStart.y + t * (wEnd.y - wStart.y);

                        if (transform.pos.y >= groundY && (transform.pos.y - groundY) < detectionThreshold) {
                            transform.pos.y = groundY;
                            foundGround = true;

                            // --- CALCUL DU VECTEUR DE PENTE ---
                            sf::Vector2f diff = wEnd - wStart;
                            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                            sf::Vector2f slopeDir = diff / length; // Vecteur unitaire de la pente

                            // --- LOGIQUE DE VITESSE ---
                            float currentSpeed = std::sqrt(transform.velocity.x * transform.velocity.x +
                                transform.velocity.y * transform.velocity.y);

                            if (seg.type == SlopeType::DOWN) {
                                // Accélération en descente (Zone Bleue)
                                // Plus on plonge (GravityMultiplier), plus on gagne en vitesse de pointe
                                float boost = 400.f * (GravityMultiplier / 2.0f);
                                currentSpeed += boost * dt;
                            }
                            else { // SlopeType::UP (Zone Rouge)
                                // Ralentissement en montée
                                currentSpeed -= 1000.f * dt;
                                if (currentSpeed < 100.f) currentSpeed = 100.f; // Vitesse mini pour ne pas bloquer
                            }

                            // On réaligne la vélocité sur la pente pour "coller" au sol
                            transform.velocity = slopeDir * currentSpeed;
                        }
                        break;
                    }
                }
            }
        }
        wasPressedLastFrame = isPressed;
    }
}