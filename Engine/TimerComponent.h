#pragma once
#include "Component.h"
#include <iostream>

class TimerComponent : public Component {
public:
    // Le temps de survie alloué pour chaque surface (en secondes)
    float hillTimer = 15.0f;
    float cloudTimer = 15.0f;

    // États mis à jour par ton système de physique/collision
    bool isTouchingHill = false;
    bool isTouchingCloud = false;

    // Sécurité pour ne déclencher la fin qu'une seule fois
    bool isGameOver = false;

    void Update(float dt) override {
        if (isGameOver) return; // Si c'est perdu, on stoppe le décompte

        // 1. Décrémentation selon la surface touchée
        if (isTouchingHill) {
            hillTimer -= dt;
        }
        if (isTouchingCloud) {
            cloudTimer -= dt;
        }

        // --- OPTIONNEL : Régénération ---
        // (Décommente si tu veux que la jauge remonte quand on n'y est pas)
        // if (!isTouchingHill && hillTimer < 15.0f) hillTimer += dt * 0.5f;
        // if (!isTouchingCloud && cloudTimer < 15.0f) cloudTimer += dt * 0.5f;

        // 2. Vérification du Game Over
        if (hillTimer <= 0.0f || cloudTimer <= 0.0f) {
            TriggerGameOver();
        }
    }

    void TriggerGameOver() {
        isGameOver = true;
        std::cout << "GAME OVER ! Le joueur est reste trop longtemps sur une surface." << std::endl;

        // Logique de fin de partie :
        // owner->GetScene()->Restart(); 
    }
};