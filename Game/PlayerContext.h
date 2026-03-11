#pragma once
#include <SpriteRendererComponent.h>

class GameObject;

class PlayerContext
{
public:
	GameObject* player = nullptr;
	SpriteRendererComponent* playerSprite = nullptr;

};