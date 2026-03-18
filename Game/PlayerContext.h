#pragma once

class GameObject;
class SpriteRendererComponent;

class PlayerContext
{
public:
	GameObject* player = nullptr;
	SpriteRendererComponent* playerSprite = nullptr;

};