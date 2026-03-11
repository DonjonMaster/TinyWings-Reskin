#pragma once
#include "Component.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class HillComponent : public Component {
public:
    HillComponent() {}

    void Init(sf::Vector2f start, sf::Vector2f end, SlopeType type);
    void InitFromImage(const std::string& texturePath, int precision = 10);

    void Render(sf::RenderWindow* window) override;

    struct Segment {
        sf::Vector2f start;
        sf::Vector2f end;
        // Initialisation par défaut pour corriger l'erreur type.6
        SlopeType type = SlopeType::UP;
    };

    const std::vector<Segment>& GetSegments() const { return segments; }
    sf::Vector2f GetWorldPos(sf::Vector2f localPos) const;

private:
    // Utilisation de pointeurs pour s'adapter à SFML 3.0
    std::unique_ptr<sf::Sprite> sprite;
    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
    std::vector<Segment> segments;
    bool hasImage = false;
};