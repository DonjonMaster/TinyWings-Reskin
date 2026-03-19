#include "Background.h"
#include "GameObject.h"
#include "Engine.h"
#include "WindowModule.h"
#include "ModuleManager.h"

Background::Background(const std::string& texturePath) {
    texture = std::make_unique<sf::Texture>();

    if (texture->loadFromFile(texturePath)) {
        // En SFML 3, on peut créer le sprite directement avec la texture
        sprite = std::make_unique<sf::Sprite>(*texture);

        sf::Vector2u size = texture->getSize();
        backgroundHeight = static_cast<float>(size.y);

        // Origine en bas au milieu
        sprite->setOrigin({ static_cast<float>(size.x) / 2.0f, backgroundHeight });
    }
}

void Background::Update(float dt) {
    if (!sprite) return;

    auto wm = Engine::GetInstance()->GetModuleManager()->GetModule<WindowModule>();
    sf::RenderWindow* window = wm->GetRenderWindow();

    sf::View view = window->getView();
    sf::Vector2f viewCenter = view.getCenter();

    // 1. POSITION X (Dynamique)
    // On suit le centre de la caméra pour que le fond soit toujours visible horizontalement.
    float posX = viewCenter.x;

    // 2. POSITION Y (Fixe)
    // On définit une coordonnée fixe dans le MONDE.
    // Si ton joueur commence à 0, mets 0. S'il commence à 600, mets 600.
    // Cette valeur ne doit JAMAIS dépendre de viewCenter.y.
    float posY = 0.0f;

    sprite->setPosition({ posX, posY });

    // 3. ZOOM / SCALE (Stable)
    // On oublie le zoom dynamique. On met une échelle fixe (ex: 1.0 ou plus).
    // On s'assure juste que l'image est assez large.
    sprite->setScale({ 1.0f, 1.0f });
}

void Background::Render(sf::RenderWindow* window) {
    if (window && sprite) {
        window->draw(*sprite);
    }
}