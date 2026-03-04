#pragma once
#include <SFML/Graphics.hpp>

class Component
{
	friend class GameObject;

public:
	virtual void Create() {};
	virtual void Update(float dt) {};
	virtual void Render(sf::RenderWindow* window) {};
	virtual void Destroy() {};

protected:
	GameObject* owner;
};