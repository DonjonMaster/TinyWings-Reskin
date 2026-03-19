#include "TimerRenderer.h"
#include "GameObject.h"
#include "Scene.h"
#include "TimerComponent.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void TimerRenderer::Create()
{
    // Initialisation manuelle
    font = std::make_unique<sf::Font>();

    // On utilise la même police que ton ScoreRenderer
    if (!font->openFromFile("Assets/Fonts/HennyPenny-Regular.ttf")) {
        std::cerr << "Erreur : Police non trouvée !" << std::endl;
        return;
    }

    // Création du texte principal
    mainText = std::make_unique<sf::Text>(*font);
    mainText->setCharacterSize(70);
    mainText->setOutlineThickness(3.f);

    // Création du texte secondaire
    subText = std::make_unique<sf::Text>(*font);
    subText->setCharacterSize(40);
    subText->setOutlineThickness(2.f);
    subText->setFillColor(sf::Color(180, 180, 180));
    subText->setOutlineColor(sf::Color::Black);
}

void TimerRenderer::Render(sf::RenderWindow* window)
{
    if (!window || !owner || !owner->GetScene() || !mainText || !subText) return;

    // Récupération du joueur
    GameObject* player = nullptr;
    for (auto* obj : owner->GetScene()->GetGameObjects()) {
        if (obj->GetName() == "Player") {
            player = obj;
            break;
        }
    }

    if (!player) return;

    // Récupération des timers
    auto* timers = player->GetComponent<TimerComponent>();
    if (!timers) return;

    // Logique d'affichage selon ce qu'on touche
    if (timers->isTouchingHill) {
        isHillMain = true;
    }
    else if (timers->isTouchingCloud) {
        isHillMain = false;
    }

    std::ostringstream hillStream, cloudStream;
    hillStream << std::fixed << std::setprecision(2) << timers->hillTimer;
    cloudStream << std::fixed << std::setprecision(2) << timers->cloudTimer;

    if (isHillMain) {
        mainText->setString(hillStream.str());
        mainText->setFillColor(sf::Color(50, 200, 50));
        mainText->setOutlineColor(sf::Color(20, 100, 20));

        subText->setString(cloudStream.str());
    }
    else {
        mainText->setString(cloudStream.str());
        mainText->setFillColor(sf::Color(100, 200, 255));
        mainText->setOutlineColor(sf::Color(20, 50, 150));

        subText->setString(hillStream.str());
    }

    // Centrage à l'écran
    float windowWidth = static_cast<float>(window->getSize().x);

    sf::FloatRect mainBounds = mainText->getLocalBounds();
    mainText->setPosition({ (windowWidth / 2.0f) - (mainBounds.size.x / 2.0f), 20.0f });

    sf::FloatRect subBounds = subText->getLocalBounds();
    subText->setPosition({ (windowWidth / 2.0f) - (subBounds.size.x / 2.0f), 100.0f });

    // Affichage en vue fixe
    sf::View currentView = window->getView();
    window->setView(window->getDefaultView());

    window->draw(*mainText);
    window->draw(*subText);

    window->setView(currentView);
}