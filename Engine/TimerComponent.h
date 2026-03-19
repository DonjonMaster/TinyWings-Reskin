#pragma once
#include "Component.h"

class TimerComponent : public Component {
public:
    float hillTimer = 45.0f;
    float cloudTimer = 45.0f;
    bool isGameOver = false;

    // Permet au Renderer de savoir quelle zone est active
    bool isInHillZone = true;

    void Update(float dt) override;
};