#include "TimerRenderer.h"
#include "GameObject.h"
#include "Scene.h"
#include "TimerComponent.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void TimerRenderer::Create()
{
    font = std::make_shared<sf::Font>();

    if (!font->openFromFile("Assets/Fonts/Bangers-Regular.ttf")) {
        std::cerr << "Erreur : Police non trouvée pour les timers !" << std::endl;
        return;
    }

    mainText = std::make_shared<sf::Text>(*font);
    mainText->setCharacterSize(70);
    mainText->setOutlineThickness(3.f);

    subText = std::make_shared<sf::Text>(*font);
    subText->setCharacterSize(40);
    subText->setOutlineThickness(2.f);
    subText->setFillColor(sf::Color(180, 180, 180));
    subText->setOutlineColor(sf::Color::Black);
}

void TimerRenderer::Render(sf::RenderWindow* window)
{
    if (!window || !owner || !owner->GetScene() || !mainText || !subText) return;

    GameObject* player = nullptr;
    for (auto* obj : owner->GetScene()->GetGameObjects()) {
        if (obj->GetName() == "Player") {
            player = obj;
            break;
        }
    }

    if (!player) return;

    auto* timers = player->GetComponent<TimerComponent>();
    if (!timers) return;

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

    float windowWidth = static_cast<float>(window->getSize().x);

    // SFML 3 : .size.x et sf::Vector2f
    sf::FloatRect mainBounds = mainText->getLocalBounds();
    mainText->setPosition(sf::Vector2f((windowWidth / 2.0f) - (mainBounds.size.x / 2.0f), 20.0f));

    sf::FloatRect subBounds = subText->getLocalBounds();
    subText->setPosition(sf::Vector2f((windowWidth / 2.0f) - (subBounds.size.x / 2.0f), 100.0f));

    sf::View currentView = window->getView();
    window->setView(window->getDefaultView());

    window->draw(*mainText);
    window->draw(*subText);

    window->setView(currentView);
}