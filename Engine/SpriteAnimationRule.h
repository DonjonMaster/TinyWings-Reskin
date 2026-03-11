#pragma once
#include <SFML/Graphics.hpp>

struct SpriteAnimationRule
{
	SpriteAnimationRule(int frames, sf::Vector2i pos, sf::Vector2i size) : SpriteAnimationRule(frames, pos, size, 1.f) {};
	SpriteAnimationRule(int frames, sf::Vector2i pos, sf::Vector2i size, float speed) : frames(frames), speed(speed), pos(pos), size(size) {};

	int frames;
	sf::Vector2i pos;
	sf::Vector2i size;
	float speed;
};