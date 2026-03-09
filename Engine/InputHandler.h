#pragma once
#include "Component.h"

class InputHandler : public Component {
public:
    void Update(float dt) override;
    bool IsActionPressed() const { return isPressed; }

private:
    bool isPressed = false;
};