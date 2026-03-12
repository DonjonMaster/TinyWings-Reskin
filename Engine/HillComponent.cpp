#include "HillComponent.h"
#include "GameObject.h"
#include <cmath> 

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
        sprite->setScale(owner->GetTransform().scale);
        window->draw(*sprite);
    }

    // --- PARAMÈTRES DE LA LIGNE ---
    float thickness = 8.0f; // Épaisseur totale de la ligne
    float radius = thickness / 2.0f;

    // On prépare un cercle qui servira de "jointure" pour boucher les trous
    // 12 points de précision suffisent pour un petit cercle
    sf::CircleShape joint(radius, 12);
    joint.setOrigin({ radius, radius }); // On centre le cercle sur lui-même

    for (const auto& seg : segments) {
        sf::Vector2f wStart = GetWorldPos(seg.start);
        sf::Vector2f wEnd = GetWorldPos(seg.end);
        sf::Color color = (seg.type == SlopeType::UP) ? sf::Color::Red : sf::Color::Blue;

        sf::Vector2f dir = wEnd - wStart;
        float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (length > 0.0f) {
            // Calcul du rectangle (comme avant)
            sf::Vector2f normal(-dir.y / length, dir.x / length);
            sf::Vector2f offset = normal * radius;

            sf::Vertex line[] = {
                sf::Vertex(wStart + offset, color),
                sf::Vertex(wStart - offset, color),
                sf::Vertex(wEnd + offset, color),
                sf::Vertex(wEnd - offset, color)
            };

            // 1. On dessine le segment (le trait)
            window->draw(line, 4, sf::PrimitiveType::TriangleStrip);

            // 2. On dessine un cercle au début et à la fin pour arrondir les angles !
            joint.setFillColor(color);

            joint.setPosition(wStart);
            window->draw(joint);

            joint.setPosition(wEnd);
            window->draw(joint);
        }
    }
}