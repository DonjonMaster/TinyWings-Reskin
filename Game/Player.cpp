#include "Player.h"
#include "GameObject.h"
#include "Constants.h"
#include "GravityComponent.h"
#include "InputHandler.h"
#include <iostream>
#include <algorithm>

void Player::Create() {
    GravityMultiplier = 1.0f;
    owner->GetTransform().velocity = PlayerSettings::START_VELOCITY;
}

void Player::Update(float dt) {
    auto* input = owner->GetComponent<InputHandler>();
    auto* grav = owner->GetComponent<GravityComponent>();

    if (input && grav) {
        bool isPressed = input->IsActionPressed();

        if (isPressed) {
            // Affichage console unique au moment de l'appui
            if (!wasPressedLastFrame) {
                std::cout << "--- Plongee activee (" << GravityMultiplier << ") -- - " << std::endl;
            }
            // On augmente la gravite depuis sa valeur actuelle
            GravityMultiplier += 4.0f * dt;
        }
        else {
            // Affichage console unique au moment du relachement
            if (wasPressedLastFrame) {
                std::cout << "--- Plongee relachee (" << GravityMultiplier << ") ---" << std::endl;
            }
            // On revient doucement a la gravite par defaut
            if (GravityMultiplier > 1.0f) {
                GravityMultiplier -= 1.5f * dt;
            }
        }

        // Bornes de securite : entre x1 et x6 la gravite de base
        GravityMultiplier = std::clamp(GravityMultiplier, 1.0f, 6.0f);

        // Mise a jour du GravityComponent de l'Engine
        float finalY = PlayerSettings::GRAVITY * GravityMultiplier;
        grav->SetGravity({ 0.f, finalY });

        wasPressedLastFrame = isPressed;
    }
}