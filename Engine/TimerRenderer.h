#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include <memory> 

class TimerRenderer : public Component
{
public:
    TimerRenderer() = default;

    void Create() override;
    void Render(sf::RenderWindow* window) override;

private:
    std::unique_ptr<sf::Font> font;
    std::unique_ptr<sf::Text> mainText;
    std::unique_ptr<sf::Text> subText;

    bool isHillMain = true;
};