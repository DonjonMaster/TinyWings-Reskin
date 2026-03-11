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

            for (GameObject* obj : allObjects) {
                auto* hill = obj->GetComponent<HillComponent>();
                if (!hill) continue;

                const auto& segments = hill->GetSegments();
                for (const auto& seg : segments) {
                    sf::Vector2f wStart = hill->GetWorldPos(seg.start);
                    sf::Vector2f wEnd = hill->GetWorldPos(seg.end);

                    // Vérifie si le joueur est horizontalement dans ce segment
                    if (transform.pos.x >= wStart.x && transform.pos.x <= wEnd.x) {
                        // Interpolation pour trouver le Y précis sur la pente
                        float t = (transform.pos.x - wStart.x) / (wEnd.x - wStart.x);
                        float groundY = wStart.y + t * (wEnd.y - wStart.y);

                        if (transform.pos.y >= groundY) {
                            transform.pos.y = groundY;
                            if (transform.velocity.y > 0) transform.velocity.y = 0;
                        }
                        break; // Segment trouvé, on sort de la boucle segments
                    }
                }
            }
        }
        wasPressedLastFrame = isPressed;
    }
}