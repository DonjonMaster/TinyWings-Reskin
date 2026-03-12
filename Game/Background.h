#pragma once
#include <Component.h>
#include <GameObject.h>
#include <vector>
#include <string>
#include <memory> 
#include "SFML/Graphics.hpp"

class BackgroundScroller : public Component
{
public:
    void Create() override;
    void Update(float dt) override;
    void Render(sf::RenderWindow* window) override;

    void LoadSection(int index);
    void PreloadChunks();

private:
    struct Layer
    {
        std::vector<sf::Texture> textures;

        // Au lieu de 2 sprites fixes, on a une liste dynamique (Buffer)
        std::vector<std::unique_ptr<sf::Sprite>> activeSprites;

        float speed = 0.f;

        // Index du prochain chunk a afficher pour cette layer quand un sprite sort de l'ecran
        int nextChunkIndex = 0;

        Layer() = default;
    };

    std::vector<std::unique_ptr<Layer>> layers;

    float chunkWidth = 800.f;
    float chunkHeight = 1000.f;

    // Nombre de chunks chargés en avance
    const int NUM_ACTIVE_CHUNKS = 5;
};