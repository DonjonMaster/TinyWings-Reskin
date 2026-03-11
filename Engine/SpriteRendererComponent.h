#pragma once
#include "Component.h"
#include "Transform.h"
#include "Engine.h"
#include "GameObject.h"
#include "ResourceModule.h"
#include "ModuleManager.h"
#include "SpriteAnimationRule.h"
#include <string>

class SpriteRendererComponent : public Component
{
private:
	sf::Sprite* sprite;
	bool isVisible;
	std::optional<SpriteAnimationRule> animationRule;
	float elapsedAnimationTime = 0.f;

public:
	SpriteRendererComponent(std::string path);
	void Update(float dt) override;
	void Render(sf::RenderWindow* window) override;

	bool SetIsVisible(bool isVisible);

	void SetAnimationRule(const SpriteAnimationRule& animationRule);
};

