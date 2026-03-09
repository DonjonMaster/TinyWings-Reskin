#pragma once
#include <SFML/Graphics.hpp>

struct Transform
{
	sf::Vector2f pos = { 0.f, 0.f };
	sf::Vector2f scale = { 1.f, 1.f };
	sf::Vector2f origin = { 0.5f, 0.5f };
	sf::Vector2f velocity = { 0.f, 0.f };
	float rotation = 0.f;
};