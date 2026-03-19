#include "ScoreRenderer.h"
#include "GameObject.h"
#include "Scene.h"
#include <iostream>

void ScoreRenderer::Create()
{
    // Initialisation manuelle des objets SFML
    font = std::make_unique<sf::Font>();

    // SFML 3.0 utilise souvent loadFromFile
    if (!font->openFromFile("Assets/Fonts/HennyPenny-Regular.ttf")) {
        std::cerr << "Erreur : Police non trouvée !" << std::endl;
        return;
    }

    // On crée le texte en lui passant la police (obligatoire en SFML 3)
    text = std::make_unique<sf::Text>(*font);
    text->setCharacterSize(40);
    text->setFillColor(sf::Color::Yellow);
    text->setOutlineColor(sf::Color::Yellow);
    text->setOutlineThickness(2.f);
    text->setPosition({ 20.f, 20.f });
}

void ScoreRenderer::Render(sf::RenderWindow* window)
{
    if (!window || !owner || !owner->GetScene() || !text) return;

    // Récupération du score depuis la scène
    int scoreToDisplay = static_cast<int>(owner->GetScene()->currentScore);
    text->setString("SCORE : " + std::to_string(scoreToDisplay));

    // Affichage en vue fixe
    sf::View currentView = window->getView();
    window->setView(window->getDefaultView());

    window->draw(*text);

    window->setView(currentView);
}