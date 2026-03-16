#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Component.h"
#include "Constants.h"

// Structure de retour pour les collisions
struct SlopeData {
    bool hit = false;
    float surfaceY = 0.f;
    sf::Vector2f direction = { 1.f, 0.f };
    SlopeType type = SlopeType::DOWN;
};

struct Segment {
    sf::Vector2f start;
    sf::Vector2f end;
    SlopeType type;
};

class HillComponent : public Component {
public:
    // Initialisation par défaut avec isOneWay à false
    HillComponent() : texture(std::make_shared<sf::Texture>()), hasImage(false), isOneWay(false) {}

    void Init(sf::Vector2f start, sf::Vector2f end, SlopeType type);
    void InitFromImage(const std::string& texturePath, int precision = 10);

    // Fonction de collision
    SlopeData GetSlopeAt(float worldX);

    sf::Vector2f GetWorldPos(sf::Vector2f localPos) const;
    void Render(sf::RenderWindow* window) override;

    const std::vector<Segment>& GetSegments() const { return segments; }

    float collisionThickness = 100.0f;
    bool isOneWay;
    bool showDebugCollision = false;

private:
    std::vector<Segment> segments;
    std::shared_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> sprite;
    bool hasImage;
};