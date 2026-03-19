#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <memory> // Pour std::unique_ptr
#include <string>

class Background : public Component
{
private:
    std::unique_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> sprite;
    float backgroundHeight = 0.0f;

public:
    // Constructeur
    Background(const std::string& texturePath);

    void Update(float dt) override;
    void Render(sf::RenderWindow* window) override;
};