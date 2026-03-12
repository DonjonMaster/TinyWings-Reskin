#include "Background.h"
#include <iostream>

void BackgroundScroller::Create()
{
    PreloadChunks();
}

void BackgroundScroller::PreloadChunks()
{
    // modif de vitesse
    const float speeds[5] = { 40.f, 70.f, 100.f, 130.f, 180.f };

    layers.clear();

    for (int layerIndex = 0; layerIndex < 5; layerIndex++)
    {
        auto layer = std::make_unique<Layer>();
        layer->speed = speeds[layerIndex];
        layer->textures.resize(8);

        for (int chunk = 0; chunk < 8; chunk++)
        {
            std::string path = "Assets/AssetsBackground/" + std::to_string(chunk + 1) + "/" + std::to_string(layerIndex + 1) + ".png";
            if (!layer->textures[chunk].loadFromFile(path))
            {
                std::cerr << "ERREUR : " << path << std::endl;
            }
        }

        for (int i = 0; i < NUM_ACTIVE_CHUNKS; i++)
        {
            int textureIndex = i % 8;

            auto sprite = std::make_unique<sf::Sprite>(layer->textures[textureIndex]);

            sprite->setScale(sf::Vector2f(3.f, 3.f));
            sprite->setPosition(sf::Vector2f(i * chunkWidth, 0.f));

            layer->activeSprites.push_back(std::move(sprite));
        }

        layer->nextChunkIndex = NUM_ACTIVE_CHUNKS % 8;

        layers.push_back(std::move(layer));
    }
}

void BackgroundScroller::LoadSection(int index)
{
    for (auto& layer : layers)
    {
        for (int i = 0; i < NUM_ACTIVE_CHUNKS; i++)
        {
            int texIndex = (index + i) % 8;
            layer->activeSprites[i]->setTexture(layer->textures[texIndex]);
            layer->activeSprites[i]->setPosition(sf::Vector2f(i * chunkWidth, 0.f));
        }
        layer->nextChunkIndex = (index + NUM_ACTIVE_CHUNKS) % 8;
    }
}

void BackgroundScroller::Update(float dt)
{

    for (auto& layer : layers)
    {
        for (auto& sprite : layer->activeSprites)
        {
            sprite->move(sf::Vector2f(-layer->speed * dt, 0.f));
        }

        for (auto& sprite : layer->activeSprites)
        {
            if (sprite->getPosition().x + chunkWidth <= 0.f)
            {
                float maxRightX = -99999.f;
                for (auto& s : layer->activeSprites)
                {
                    if (s->getPosition().x > maxRightX)
                        maxRightX = s->getPosition().x;
                }

                sprite->setPosition(sf::Vector2f(maxRightX + chunkWidth, 0.f));

                sprite->setTexture(layer->textures[layer->nextChunkIndex]);

                layer->nextChunkIndex = (layer->nextChunkIndex + 1) % 8;
            }
        }
    }
}

void BackgroundScroller::Render(sf::RenderWindow* window)
{
    if (!owner) return;
    sf::Vector2f basePos = owner->GetTransform().pos;

    for (auto& layer : layers)
    {
        for (auto& sprite : layer->activeSprites)
        {
            if (sprite) {
                // On ajoute la position du GameObject à la position relative du sprite
                sf::Vector2f relativePos = sprite->getPosition();
                sprite->setPosition(basePos + relativePos);

                window->draw(*sprite);

                // On remet la position relative pour ne pas fausser l'Update au prochain tour
                sprite->setPosition(relativePos);
            }
        }
    }
}