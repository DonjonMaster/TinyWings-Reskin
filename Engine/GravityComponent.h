#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

class GravityComponent : public Component {
public:
    void Update(float dt) override;
    void SetGravity(sf::Vector2f newGravity) { gravity = newGravity; }

private:
    sf::Vector2f gravity = { 0.f, 981.f };
};