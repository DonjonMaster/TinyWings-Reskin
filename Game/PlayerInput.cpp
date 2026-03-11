#include "PlayerInput.h"
#include "GameObject.h"      // Corrige "type incomplet"
#include "Constants.h"
#include "GravityComponent.h"
#include "InputHandler.h"
#include "HillComponent.h"   // Corrige "GetWorldStart n'est pas membre"
#include <Scene.h>           // Indispensable pour accéder à la scène
#include <iostream>
#include <algorithm>
#include <vector>            // Indispensable pour allObjects

void DivingInput::Create() {
    GravityMultiplier = 1.0f;
    owner->GetTransform().velocity = PlayerSettings::START_VELOCITY;
}

void DivingInput::Update(float dt) {
    auto* input = owner->GetComponent<InputHandler>();
    auto* grav = owner->GetComponent<GravityComponent>();
    auto& transform = owner->GetTransform();

    if (input && grav) {
        // --- Logique Gravité ---
        bool isPressed = input->IsActionPressed();
        if (isPressed) GravityMultiplier += 9.0f * dt;
        else if (GravityMultiplier > 1.0f) GravityMultiplier -= 1.5f * dt;
        GravityMultiplier = std::clamp(GravityMultiplier, 1.0f, 6.0f);
        grav->SetGravity({ 0.f, PlayerSettings::GRAVITY * GravityMultiplier });

        // --- Logique Collision Auto-Hill ---
        Scene* currentScene = owner->GetScene();
        if (currentScene) {
            const auto& allObjects = currentScene->GetGameObjects();

            // On autorise un enfoncement maximum de 150 pixels par frame.
            // Au-delà, l'algorithme considèrera que c'est une colline au-dessus du joueur (un plafond)
            float minPenetration = 150.0f;
            float bestGroundY = 0.0f;
            bool foundGround = false;

            // ÉTAPE A : ANALYSE (Trouver le sol sous les pieds du joueur)
            for (GameObject* obj : allObjects) {
                auto* hill = obj->GetComponent<HillComponent>();
                if (!hill) continue;

                const auto& segments = hill->GetSegments();
                for (const auto& seg : segments) {
                    sf::Vector2f wStart = hill->GetWorldPos(seg.start);
                    sf::Vector2f wEnd = hill->GetWorldPos(seg.end);

                    if (transform.pos.x >= wStart.x && transform.pos.x <= wEnd.x) {
                        float t = (transform.pos.x - wStart.x) / (wEnd.x - wStart.x);
                        float groundY = wStart.y + t * (wEnd.y - wStart.y);

                        // Calcul de la distance entre le joueur et ce sol
                        float penetration = transform.pos.y - groundY;

                        // Si le joueur est sous le sol (penetration >= 0) 
                        // ET qu'il n'est pas absurdement loin sous le sol (penetration < minPenetration)
                        if (penetration >= 0.0f && penetration < minPenetration) {
                            minPenetration = penetration; // On garde la surface la plus proche de lui
                            bestGroundY = groundY;
                            foundGround = true;
                        }
                        break; // On a trouvé la pente pour cette colline, on passe à la suivante
                    }
                }
            }

            // ÉTAPE B : RÉSOLUTION (On replace le joueur)
            if (foundGround) {
                transform.pos.y = bestGroundY;
                if (transform.velocity.y > 0) {
                    transform.velocity.y = 0; // On annule la chute
                }
            }
        }
        wasPressedLastFrame = isPressed;
    }
}