#include "SpriteRendererComponent.h"

SpriteRendererComponent::SpriteRendererComponent(std::string path)
	: sprite(nullptr), isVisible(true) // Initialisation propre
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
