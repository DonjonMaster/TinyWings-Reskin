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
        float bestSurfaceY = 0.0f;
        sf::Vector2f bestSlopeDir;
        SlopeType bestSlopeType = SlopeType::UP;
        bool foundGroundThisFrame = false;
        float minDistance = 999999.0f;

        for (GameObject* obj : currentScene->GetGameObjects()) {
            auto* hill = obj->GetComponent<HillComponent>();
            if (!hill) continue;

            for (const auto& seg : hill->GetSegments()) {
                sf::Vector2f wStart = hill->GetWorldPos(seg.start);
                sf::Vector2f wEnd = hill->GetWorldPos(seg.end);

                // CORRECTIF TROU : On utilise une marge de 1.0f pour les pentes raides
                float minX = std::min(wStart.x, wEnd.x) - 1.0f;
                float maxX = std::max(wStart.x, wEnd.x) + 1.0f;

                if (transform.pos.x >= minX && transform.pos.x <= maxX) {
                    float t = (transform.pos.x - wStart.x) / (wEnd.x - wStart.x);
                    t = std::clamp(t, 0.0f, 1.0f);
                    float surfaceY = wStart.y + t * (wEnd.y - wStart.y);

                    // UTILISATION DE LA VRAIE VALEUR DU COMPOSANT
                    float snapMargin = 15.0f;
                    if (transform.pos.y >= surfaceY - snapMargin &&
                        transform.pos.y <= surfaceY + hill->collisionThickness) {

                        // Sécurité envol
                        if (transform.velocity.y < -100.0f && transform.pos.y < surfaceY) continue;

                        // Sélection de la plateforme la plus proche (évite la téléportation en haut)
                        float dist = std::abs(transform.pos.y - surfaceY);
                        if (dist < minDistance) {
                            minDistance = dist;
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
            // ... (calcul de vitesse habituel) ...
            float speed = std::sqrt(transform.velocity.x * transform.velocity.x + transform.velocity.y * transform.velocity.y);
            speed += (bestSlopeType == SlopeType::DOWN ? (isPressed ? 800 : 400) : (isPressed ? -800 : -1500)) * dt;
            if (speed < 150.f) speed = 150.f;
            transform.velocity = bestSlopeDir * speed;
        }
        else {
            isGrounded = false;
        }
    }
}