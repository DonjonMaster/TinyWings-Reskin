#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "GameObject.h"

class ScoreRenderer : public Component
{
public:
    ScoreRenderer() = default;

    //void Create() override;
    //void Update(float dt) override;
    //void Render(sf::RenderWindow* window) override;

    // Fonction pour mettre à jour la valeur du score depuis l'extérieur
    void SetScore(int newScore) { scoreValue = newScore; }

private:
    sf::Font font;
    sf::Text scoreText;
    int scoreValue = 0;
};