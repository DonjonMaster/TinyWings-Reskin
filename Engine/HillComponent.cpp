#include "HillComponent.h"
#include "GameObject.h" // Indispensable pour GetTransform()

void HillComponent::Init(sf::Vector2f start, sf::Vector2f end, SlopeType type) {
    this->localStart = start;
    this->localEnd = end;
    this->slopeType = type;
    this->debugColor = (type == SlopeType::UP) ? sf::Color::Red : sf::Color::Blue;
}

void HillComponent::Render(sf::RenderWindow* window) {
    if (!owner) return;
    sf::Vector2f start = GetWorldStart();
    sf::Vector2f end = GetWorldEnd();

    sf::Vertex line[] = { sf::Vertex(start, debugColor), sf::Vertex(end, debugColor) };
    window->draw(line, 2, sf::PrimitiveType::Lines);
}

sf::Vector2f HillComponent::GetWorldStart() const {
    return owner->GetTransform().pos + localStart;
}

sf::Vector2f HillComponent::GetWorldEnd() const {
    return owner->GetTransform().pos + localEnd;
}