#include <iostream>
#include "Background.h"

void BackgroundScroller::Create()
{
    PreloadChunks();
}

void BackgroundScroller::PreloadChunks()
{
    layers.clear();
    sf::Vector2u windowSize = { 1280, 720 }; // taille de l'ecran

    // une seule couche
    for (int layerIndex = 0; layerIndex < 1; layerIndex++)
    {
        auto layer = std::make_unique<Layer>();
        layer->textures.resize(1);

        std::string path = "Assets/AssetsBackground/origbig.png";
        if (!layer->textures[0].loadFromFile(path))
        {
            std::cerr << "ERREUR : Impossible de charger " << path << std::endl;
            continue;
        }

        auto sprite = std::make_unique<sf::Sprite>(layer->textures[0]);

        // --- Calcul de l'échelle pour remplir l'écran ---
        sf::Vector2u texSize = layer->textures[0].getSize();
        float scaleX = (float)windowSize.x / texSize.x;
        float scaleY = (float)windowSize.y / texSize.y;
        sprite->setScale(sf::Vector2f(scaleX, scaleY));

        layer->activeSprites.push_back(std::move(sprite));
        layers.push_back(std::move(layer));
    }
}

void BackgroundScroller::Render(sf::RenderWindow* window)
{
    if (!window) return;

    // sauvegarde de la vue de la caméra qui suit le joueur
    sf::View currentView = window->getView();

    window->setView(window->getDefaultView());

    // dessin des couches de fond
    for (auto& layer : layers)
    {
        for (auto& sprite : layer->activeSprites)
        {
            if (sprite) window->draw(*sprite);
        }
    }

    // restauration de la vue caméra pour dessiner le joueur et les collines par-dessus
    window->setView(currentView);
}