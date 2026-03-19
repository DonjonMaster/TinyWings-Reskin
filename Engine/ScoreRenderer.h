#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include <memory> 

class ScoreRenderer : public Component
{
public:
    ScoreRenderer() = default;

    void Create() override;
    void Render(sf::RenderWindow* window) override;

private:
    std::unique_ptr<sf::Font> font;
    std::unique_ptr<sf::Text> text;
};