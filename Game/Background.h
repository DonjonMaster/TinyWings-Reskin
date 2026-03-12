#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <Component.h>
#include <GameObject.h>
#include <Scene.h>



class BackgroundScroller : public Component
{
public:
    void Create() override;
    void Render(sf::RenderWindow* window) override;

    void PreloadChunks();

private:
    struct Layer
    {
        std::vector<sf::Texture> textures;

        // au cas ou on veut superposé des images pour un effet de profondeur 
        std::vector<std::unique_ptr<sf::Sprite>> activeSprites;

        Layer() = default;
    };

    std::vector<std::unique_ptr<Layer>> layers;
};