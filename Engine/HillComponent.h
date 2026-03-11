#pragma once
#include "Component.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>

class HillComponent : public Component {
public:
    // On initialise le segment
    void Init(sf::Vector2f start, sf::Vector2f end, SlopeType type);
    void Render(sf::RenderWindow* window) override;

    // AJOUT DES GETTERS (pour corriger les erreurs 'n'est pas membre de')
    SlopeType GetSlopeType() const { return slopeType; }
    sf::Vector2f GetWorldStart() const;
    sf::Vector2f GetWorldEnd() const;

private:
    sf::Vector2f localStart;
    sf::Vector2f localEnd;
    SlopeType slopeType; // Renommé 'type' en 'slopeType' pour éviter la redéfinition
    sf::Color debugColor;
};