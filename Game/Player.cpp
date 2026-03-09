#include "Player.h"
#include "GameObject.h"
#include "Constants.h"
#include "GravityComponent.h"
#include "InputHandler.h"
#include <iostream>
#include <algorithm>

void Player::Create() {
    GravityMultiplier = 1.0f;
    // Vitesse initiale pour avancer
    owner->GetTransform().velocity = PlayerSettings::START_VELOCITY;
}

void Player::Update(float dt) {
    auto* input = owner->GetComponent<InputHandler>();
    auto* grav = owner->GetComponent<GravityComponent>();

    if (input && grav) {
        bool isPressed = input->IsActionPressed();

        if (isPressed) {
            // --- LOGIQUE DE PRESSION (AUGMENTATION) ---

            // On détecte le nouvel appui pour la console
            if (!wasPressedLastFrame) {
                std::cout << "Appui detecte : Acceleration de la chute depuis x" << GravityMultiplier << std::endl;
            }

            // Plus on reste appuyé, plus on ajoute de la force au multiplicateur actuel
            // La valeur 3.0f contrôle la vitesse à laquelle le joueur "devient lourd"
            GravityMultiplier += 3.0f * dt;

        }
        else {
            // --- LOGIQUE DE RELACHEMENT (DIMINUTION) ---

            if (wasPressedLastFrame) {
                std::cout << "Touche relachee : Amortissement de la gravite..." << std::endl;
            }

            // On revient progressivement vers 1.0 (Gravité par défaut)
            // La valeur 2.0f contrôle la vitesse de "remontée" ou d'allègement
            if (GravityMultiplier > 1.0f) {
                GravityMultiplier -= 2.0f * dt;
            }
        }

        // Sécurité : On borne le multiplicateur (ex: entre 1x et 5x la gravité)
        GravityMultiplier = std::max(1.0f, std::min(GravityMultiplier, 5.0f));

        // Application de la nouvelle gravité au composant
        float finalGravityY = PlayerSettings::GRAVITY * GravityMultiplier;
        grav->SetGravity({ 0.f, finalGravityY });

        // Sauvegarde de l'état pour la détection de changement
        wasPressedLastFrame = isPressed;
    }
}