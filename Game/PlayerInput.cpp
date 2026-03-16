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
    owner->GetTransform().origin = { 0.5f, 1.0f }; // CRUCIAL : Point de collision aux pieds
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

        // ON CALCULE LA POSITION EXACTE DE LA FRAME PRECEDENTE
        float prevX = transform.pos.x - (transform.velocity.x * dt);
        float prevY = transform.pos.y - (transform.velocity.y * dt);

        for (GameObject* obj : currentScene->GetGameObjects()) {
            auto* hill = obj->GetComponent<HillComponent>();
            if (!hill) continue;

            for (const auto& seg : hill->GetSegments()) {
                sf::Vector2f wS = hill->GetWorldPos(seg.start);
                sf::Vector2f wE = hill->GetWorldPos(seg.end);

                // Marge X
                float minX = std::min(wS.x, wE.x) - 1.0f;
                float maxX = std::max(wS.x, wE.x) + 1.0f;

                if (transform.pos.x >= minX && transform.pos.x <= maxX) {
                    float t = (transform.pos.x - wS.x) / (wE.x - wS.x);
                    t = std::clamp(t, 0.0f, 1.0f);
                    float surfaceY = wS.y + t * (wE.y - wS.y);

                    // Calcul de la surface à la frame précédente
                    float rangeX = wE.x - wS.x;
                    float prevSurfaceY = surfaceY;
                    if (std::abs(rangeX) > 0.0001f) {
                        float prevT = (prevX - wS.x) / rangeX;
                        prevSurfaceY = wS.y + prevT * (wE.y - wS.y);
                    }

                    // --- LA REGLE MAGIQUE (FRANCHISSEMENT DE LIGNE) ---
                    // Est-ce que le joueur est passé d'AU-DESSUS de la ligne à EN-DESSOUS de la ligne ?
                    // (+5.0f de marge en haut et -15.0f en bas pour éviter les bugs liés à la vitesse)
                    bool crossedFromAbove = (prevY <= prevSurfaceY + 5.0f) && (transform.pos.y >= surfaceY - 15.0f);

                    // SI ON EST EN L'AIR ET QU'ON N'A PAS FRANCHI LA LIGNE PAR LE HAUT = ON IGNORE.
                    // (C'est ceci qui détruit l'effet d'aspiration de la flèche violette)
                    if (!isGrounded && !crossedFromAbove) {
                        continue;
                    }

                    // On vérifie qu'on est bien dans la zone d'épaisseur pour ne pas tomber à l'infini
                    if (transform.pos.y <= surfaceY + hill->collisionThickness) {

                        // Sécurité envol classique
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

        // --- 3. Résolution ---
        if (foundGroundThisFrame) {
            transform.pos.y = bestSurfaceY;
            isGrounded = true;

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