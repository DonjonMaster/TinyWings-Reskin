#pragma once
#include "Component.h"

class Player : public Component {
public:
    void Create() override;
    void Update(float dt) override;

private:
    float GravityMultiplier = 1.0f; // Commence à x1 (Normal)
    bool wasPressedLastFrame = false;
};