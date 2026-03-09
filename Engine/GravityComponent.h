#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

class GravityComponent : public Component
{
private:
	sf::Vector2f gravity = { 0.f, 981.f };

public:

	void Update(float dt) override;
};

