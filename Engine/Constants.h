#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>

// INPUTS
#define SPACE sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
#define LMOUSE sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)

// PLAYER SETTINGS
namespace PlayerSettings {
    inline const sf::Vector2f START_VELOCITY = { 200.f, 0.f };
    inline const float GRAVITY = 300.f;
    inline const float DIVE_FORCE = 9999.f;
}

// HILLSLOPE COLLISION
enum class SlopeType {
    UP,   // Montée (partie gauche) -> Rouge
    DOWN  // Descente (partie droite) -> Bleu
};
#endif