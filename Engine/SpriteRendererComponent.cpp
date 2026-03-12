#include "SpriteRendererComponent.h"

#define ANIMATION_SPEED 6.f

SpriteRendererComponent::SpriteRendererComponent(std::string path)
	: sprite(nullptr), isVisible(true) // Initialisation
{
	auto resModule = Engine::GetModuleManager()->GetModule<ResourceModule>();
	if (resModule) {
		sf::Texture& tex = resModule->GetTexture(path);
		sprite = new sf::Sprite(tex);
	}
	else {
		std::cout << "ERREUR : ResourceModule introuvable !" << std::endl;
	}
}

void SpriteRendererComponent::Update(float dt)
{
	// Si le sprite à une règle d'animation, alors elle sera appliqué
	if (animationRule) {
		auto rule = animationRule.value();
		elapsedAnimationTime += dt * rule.speed;
		int frame = int(elapsedAnimationTime * ANIMATION_SPEED) % (rule.frames);
		sprite->setTextureRect({ { rule.pos.x + frame * rule.size.x, rule.pos.y }, rule.size });

	}

	Transform& transform = owner->GetTransform();
	auto spriteBounds = sprite->getLocalBounds().size;
	sprite->setOrigin({ spriteBounds.x * transform.origin.x, spriteBounds.y * transform.origin.y });
	sprite->setPosition(transform.pos);
	sprite->setScale(transform.scale);

}

void SpriteRendererComponent::Render(sf::RenderWindow* window)
{

	if (isVisible)
		window->draw(*sprite);

}

bool SpriteRendererComponent::SetIsVisible(bool isVisible)
{
	return this->isVisible = isVisible;
}

void SpriteRendererComponent::SetAnimationRule(const SpriteAnimationRule& animationRule)
{

	this->animationRule = animationRule;
	this->elapsedAnimationTime = 0.f;

}