#include "GravityComponent.h"
#include "GameObject.h"

void GravityComponent::Update(float dt) {

    // activé ou desactivé la gravité 
    if (!enableGravity && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) enableGravity = true;
    else if (enableGravity && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) enableGravity = false;

    // si c'est activé on l'update
    if (owner && enableGravity) {
        auto& transform = owner->GetTransform();

        transform.velocity += gravity * dt;
        transform.pos += transform.velocity * dt;
    }
}

void GravityComponent::SetGravity(sf::Vector2f newGravity)
{
    if (enableGravity)
    {
        this->gravity = newGravity;
    }
}

sf::Vector2f GravityComponent::GetGravity()
{
    return this->gravity;
}