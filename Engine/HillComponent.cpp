#include "HillComponent.h"
#include "GameObject.h"
#include <cmath>
#include <algorithm>

void HillComponent::Init(sf::Vector2f start, sf::Vector2f end, SlopeType type) {
    Segment seg;
    seg.start = start;
    seg.end = end;
    seg.type = type;
    segments.push_back(seg);
    hasImage = false;
}

// Utilitaire interne pour Catmull-Rom
sf::Vector2f GetCatmullRom(float t, sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3) {
    float t2 = t * t;
    float t3 = t2 * t;
    return 0.5f * ((2.f * p1) + (-p0 + p2) * t + (2.f * p0 - 5.f * p1 + 4.f * p2 - p3) * t2 + (-p0 + 3.f * p1 - 3.f * p2 + p3) * t3);
}

void HillComponent::InitFromImage(const std::string& texturePath, int precision) {
    sf::Image image;
    if (!image.loadFromFile(texturePath)) return;
    if (!texture->loadFromImage(image)) return;

    sprite = std::make_unique<sf::Sprite>(*texture);
    hasImage = true;
    segments.clear(); // CRITIQUE : Nettoie les anciens segments pour le recyclage

    sf::Vector2u size = image.getSize();

    std::vector<std::vector<sf::Vector2f>> pointGroups;
    std::vector<sf::Vector2f> currentGroup;

    // CORRECTION ICI : On va jusqu'à size.x INCLUS pour sceller parfaitement le bord
    for (unsigned int x = 0; x <= size.x; x += precision) {

        // Si on est sur le pixel imaginaire du bord (size.x), on lit le dernier vrai pixel de l'image (size.x - 1)
        unsigned int pixelX = std::min(x, size.x - 1);

        bool foundPixelInColumn = false;
        for (unsigned int y = 0; y < size.y; ++y) {
            // Si le pixel est opaque (seuil à 128)
            if (image.getPixel({ pixelX, y }).a > 128) {
                // On enregistre à la coordonnée `x` (qui peut valoir exactement size.x)
                currentGroup.push_back({ (float)x, (float)y });
                foundPixelInColumn = true;
                break;
            }
        }

        if (!foundPixelInColumn && !currentGroup.empty()) {
            if (currentGroup.size() >= 2) {
                pointGroups.push_back(currentGroup);
            }
            currentGroup.clear();
        }

        // Astuce : Si notre 'precision' (ex: 10) fait qu'on allait rater le bord exact de l'image,
        // On force la prochaine itération à tomber PILE sur le bord (size.x).
        if (x < size.x && x + precision > size.x) {
            x = size.x - precision;
        }
    }

    // Ne pas oublier le dernier groupe
    if (currentGroup.size() >= 2) {
        pointGroups.push_back(currentGroup);
    }

    // Génération des segments pour chaque groupe
    for (auto& pts : pointGroups) {
        std::vector<sf::Vector2f> ctrl = pts;
        ctrl.insert(ctrl.begin(), pts.front());
        ctrl.push_back(pts.back());

        for (size_t i = 1; i < ctrl.size() - 2; ++i) {
            sf::Vector2f lastPos = ctrl[i];
            for (int s = 1; s <= 5; ++s) {
                float t = s / 5.f;
                sf::Vector2f newPos = GetCatmullRom(t, ctrl[i - 1], ctrl[i], ctrl[i + 1], ctrl[i + 2]);

                Segment seg;
                seg.start = lastPos;
                seg.end = newPos;
                seg.type = (seg.end.y < seg.start.y) ? SlopeType::UP : SlopeType::DOWN;
                segments.push_back(seg);
                lastPos = newPos;
            }
        }
    }
}

SlopeData HillComponent::GetSlopeAt(float worldX) {
    SlopeData data;
    if (!owner) return data;

    sf::Vector2f scale = owner->GetTransform().scale;
    sf::Vector2f pos = owner->GetTransform().pos;

    float localX = (worldX - pos.x) / scale.x;

    for (const auto& seg : segments) {
        float minX = std::min(seg.start.x, seg.end.x);
        float maxX = std::max(seg.start.x, seg.end.x);

        if (localX >= minX - 0.1f && localX <= maxX + 0.1f) {
            float range = seg.end.x - seg.start.x;
            float t = (std::abs(range) > 0.0001f) ? (localX - seg.start.x) / range : 0.f;
            t = std::clamp(t, 0.f, 1.f);

            float localY = seg.start.y + t * (seg.end.y - seg.start.y);

            data.hit = true;
            data.surfaceY = pos.y + (localY * scale.y);

            sf::Vector2f diff = seg.end - seg.start;
            float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            data.direction = diff / (len > 0 ? len : 1.f);

            if (data.direction.x < 0) data.direction = -data.direction;
            data.type = seg.type;

            return data;
        }
    }
    return data;
}

sf::Vector2f HillComponent::GetWorldPos(sf::Vector2f localPos) const {
    if (!owner) return localPos;
    sf::Vector2f scale = owner->GetTransform().scale;
    return owner->GetTransform().pos + sf::Vector2f(localPos.x * scale.x, localPos.y * scale.y);
}

void HillComponent::Render(sf::RenderWindow* window) {
    if (!owner) return;

    if (hasImage && sprite) {
        sprite->setPosition(owner->GetTransform().pos);
        sprite->setScale(owner->GetTransform().scale);
        window->draw(*sprite);
    }

    if (showDebugCollision) {
        float lineThickness = 4.0f;

        for (const auto& seg : segments) {
            sf::Vector2f wS = GetWorldPos(seg.start);
            sf::Vector2f wE = GetWorldPos(seg.end);

            sf::Color surfaceCol = (seg.type == SlopeType::UP) ? sf::Color::Red : sf::Color::Blue;
            sf::Color zoneCol = sf::Color(255, 0, 0, 50);

            sf::Vertex thicknessZone[] = {
                sf::Vertex(wS, zoneCol),
                sf::Vertex(wE, zoneCol),
                sf::Vertex(wE + sf::Vector2f(0.f, collisionThickness), zoneCol),
                sf::Vertex(wS + sf::Vector2f(0.f, collisionThickness), zoneCol)
            };

            sf::Vertex line[] = {
                sf::Vertex(wS, surfaceCol),
                sf::Vertex(wS + sf::Vector2f(0.f, lineThickness), surfaceCol),
                sf::Vertex(wE, surfaceCol),
                sf::Vertex(wE + sf::Vector2f(0.f, lineThickness), surfaceCol)
            };

            window->draw(thicknessZone, 4, sf::PrimitiveType::TriangleStrip);
            window->draw(line, 4, sf::PrimitiveType::TriangleStrip);
        }
    }
}