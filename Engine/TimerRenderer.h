#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include <memory>

class TimerRenderer : public Component
{
public:
    void Create() override;
    void Render(sf::RenderWindow* window) override;

private:
    // Remplacement de unique_ptr par shared_ptr
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Text> mainText;
    std::shared_ptr<sf::Text> subText;

    bool isHillMain = true;
};