#include "TimerComponent.h"
#include <iostream>

void TimerComponent::Update(float dt) {
    if (isGameOver) return; // Si c'est perdu, on stoppe le décompte

    // 1. Décrémentation selon la surface touchée
    if (isTouchingHill) {
        hillTimer -= dt;
    }
    if (isTouchingCloud) {
        cloudTimer -= dt;
    }

    // 2. Vérification du Game Over
    if (hillTimer <= 0.0f || cloudTimer <= 0.0f) {
        TriggerGameOver();
    }
}

void TimerComponent::TriggerGameOver() {
    isGameOver = true;
    std::cout << "GAME OVER ! Le joueur a epuise l'un de ses compteurs." << std::endl;

    // C'est ici que tu pourras appeler la fin de partie (ex: owner->GetScene()->Restart())
}