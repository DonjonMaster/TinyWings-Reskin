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
        // --- Ta logique de gravité actuelle (NE PAS MODIFIER) ---
        bool isPressed = input->IsActionPressed();
        if (isPressed) {
            GravityMultiplier += 9.0f * dt;
        }
        else {
            if (GravityMultiplier > 1.0f) GravityMultiplier -= 1.5f * dt;
        }
        GravityMultiplier = std::clamp(GravityMultiplier, 1.0f, 6.0f);
        float finalY = PlayerSettings::GRAVITY * GravityMultiplier;
        grav->SetGravity({ 0.f, finalY });

        // --- GESTION DES COLLISIONS ---
        Scene* currentScene = owner->GetScene();
        if (currentScene) {
            // ATTENTION : Si GetGameObjects() n'existe pas, vérifie le nom dans Scene.h
            // Si c'est une variable publique, utilise : currentScene->gameObjects
            auto& allObjects = currentScene->GetGameObjects();

            for (GameObject* obj : allObjects) {
                HillComponent* hill = obj->GetComponent<HillComponent>();
                if (!hill) continue;

                sf::Vector2f start = hill->GetWorldStart();
                sf::Vector2f end = hill->GetWorldEnd();

                // Si le joueur est au-dessus du segment de colline
                if (transform.pos.x >= start.x && transform.pos.x <= end.x) {
                    float progress = (transform.pos.x - start.x) / (end.x - start.x);
                    float hillY = start.y + progress * (end.y - start.y);

                    // Si on touche ou traverse la colline
                    if (transform.pos.y >= hillY) {
                        transform.pos.y = hillY; // On replace sur la colline
                        if (transform.velocity.y > 0) transform.velocity.y = 0;
                    }
                }
            }
        }
        wasPressedLastFrame = isPressed;
    }
}