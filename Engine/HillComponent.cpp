#include "HillComponent.h"
#include "GameObject.h"

void HillComponent::Init(sf::Vector2f start, sf::Vector2f end, SlopeType type) {
    Segment seg;
    seg.start = start;
    seg.end = end;
    seg.type = type;
    segments.push_back(seg);
    hasImage = false;
}

void HillComponent::InitFromImage(const std::string& texturePath, int precision) {
    sf::Image image;

    // on vérifie le chargement de l'image (nodiscard)
    if (!image.loadFromFile(texturePath)) {
        return;
    }

    // On vérifie le chargement de la texture (nodiscard)
    if (!texture->loadFromImage(image)) {
        return;
    }

    // creation du sprite 
    sprite = std::make_unique<sf::Sprite>(*texture);
    hasImage = true;

    sf::Vector2u size = image.getSize();
    std::vector<sf::Vector2f> points;

    for (unsigned int x = 0; x < size.x; x += precision) {
        for (unsigned int y = 0; y < size.y; ++y) {
            // Lecture des pixels compatibles SFML 3.0
            if (image.getPixel(sf::Vector2u{ x, y }).a > 128) {
                points.push_back(sf::Vector2f((float)x, (float)y));
                break;
            }
        }
    }

    segments.clear();
    for (size_t i = 0; i < points.size() - 1; ++i) {
        Segment seg;
        seg.start = points[i];
        seg.end = points[i + 1];
        seg.type = (seg.end.y < seg.start.y) ? SlopeType::UP : SlopeType::DOWN;
        segments.push_back(seg);
    }
}

sf::Vector2f HillComponent::GetWorldPos(sf::Vector2f localPos) const {
    if (!owner) return localPos;
    
    // On récupère l'échelle de notre GameObject
    sf::Vector2f scale = owner->GetTransform().scale;
    
    // On étire la position locale (les points de collision) avec l'échelle
    sf::Vector2f scaledLocalPos(localPos.x * scale.x, localPos.y * scale.y);
    
    return owner->GetTransform().pos + scaledLocalPos;
}

void HillComponent::Render(sf::RenderWindow* window) {
    if (!owner) return;

    // Si on a une image et que le sprite est bien initialisé
    if (hasImage && sprite) {
        sprite->setPosition(owner->GetTransform().pos);
        
        // AJOUT : On étire l'image selon le scale défini dans le Transform
        sprite->setScale(owner->GetTransform().scale);
        
        window->draw(*sprite);
    }

    for (const auto& seg : segments) {
        sf::Vector2f wStart = GetWorldPos(seg.start);
        sf::Vector2f wEnd = GetWorldPos(seg.end);
        sf::Color color = (seg.type == SlopeType::UP) ? sf::Color::Red : sf::Color::Blue;

        sf::Vertex line[] = { sf::Vertex(wStart, color), sf::Vertex(wEnd, color) };
        window->draw(line, 2, sf::PrimitiveType::Lines);
    }
}