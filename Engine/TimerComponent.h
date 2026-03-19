#pragma once
#include "Component.h"

class TimerComponent : public Component {
public:
    // Le temps de survie alloué pour chaque surface (en secondes)
    float hillTimer = 60.0f;
    float cloudTimer = 60.0f;

    // États mis à jour par le PlayerInput
    bool isTouchingHill = false;
    bool isTouchingCloud = false;

    // Sécurité pour ne déclencher la fin qu'une seule fois
    bool isGameOver = false;

    void Update(float dt) override;
    void TriggerGameOver();
};