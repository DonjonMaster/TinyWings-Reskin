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
    sf::Vector2f viewSize = view.getSize();

    // --- LE SECRET EST ICI ---

    // 1. Position horizontale : on suit toujours la caméra (pour pas voir les bords)
    float posX = viewCenter.x;

    // 2. Position verticale : PARALLAXE
    // Au lieu de suivre viewCenter.y à 100%, on multiplie par un facteur.
    // 0.0f = Le background ne bouge JAMAIS en Y (fixe dans le monde)
    // 0.1f = Le background bouge très lentement
    // 1.0f = Le background suit parfaitement la caméra (ton problème actuel)

    float parallaxFactor = 0.2f; // Ajuste entre 0.1 et 0.5 selon l'effet voulu
    float posY = viewCenter.y * parallaxFactor;

    // On applique la position
    // Note : Si tu as mis l'origin en bas, il faudra peut-être ajuster un offset 
    // pour qu'il commence au niveau du sol au début.
    sprite->setPosition({ posX, posY });

    // 3. Zoom (on garde ta logique qui fonctionne)
    float textureWidth = static_cast<float>(texture->getSize().x);
    float scale = viewSize.x / textureWidth;
    sprite->setScale({ scale, scale });
}

void Background::Render(sf::RenderWindow* window) {
    if (window && sprite) {
        window->draw(*sprite);
    }
}