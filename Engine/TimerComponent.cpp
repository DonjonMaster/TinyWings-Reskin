#include "TimerComponent.h"
#include "GameObject.h"

void TimerComponent::Update(float dt) {
    if (isGameOver || !owner) return;

    float playerY = owner->GetTransform().pos.y;

    // 1. ZONE ESPACE : Pause des timers
    if (playerY <= -3500.0f) {
        return;
    }

    // 2. ZONE NUAGES : La transition se fait exactement à la base des nuages
    if (playerY <= -1200.0f) {
        isInHillZone = false;
        cloudTimer -= dt;
    }
    // 3. ZONE COLLINES : Le joueur est en dessous des nuages
    else {
        isInHillZone = true;
        hillTimer -= dt;
    }

    // Vérification de la défaite
    if (hillTimer <= 0.0f || cloudTimer <= 0.0f) {
        isGameOver = true;
    }
}