#include "TimerRenderer.h"
#include "GameObject.h"
#include "Scene.h"
#include "TimerComponent.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void TimerRenderer::Create() {
    font = std::make_unique<sf::Font>();
    if (!font->openFromFile("Assets/Fonts/HennyPenny-Regular.ttf")) return;

    mainText = std::make_unique<sf::Text>(*font);
    mainText->setCharacterSize(70);
    mainText->setOutlineThickness(3.f);

    subText = std::make_unique<sf::Text>(*font);
    subText->setCharacterSize(40);
    subText->setOutlineThickness(2.f);
}

void TimerRenderer::Render(sf::RenderWindow* window) {
    if (!window || !owner) return;

    GameObject* player = nullptr;
    for (auto* obj : owner->GetScene()->GetGameObjects()) {
        if (obj->GetName() == "Player") { player = obj; break; }
    }
    if (!player) return;

    auto* timers = player->GetComponent<TimerComponent>();
    if (!timers) return;

    float playerY = player->GetTransform().pos.y;

    // La transition visuelle vers le mode SAFE se fait à la délimitation de l'espace
    bool inSpace = (playerY <= -3500.0f);

    std::ostringstream hSS, cSS;
    hSS << std::fixed << std::setprecision(2) << timers->hillTimer;
    cSS << std::fixed << std::setprecision(2) << timers->cloudTimer;

    // --- AFFICHAGE ---
    if (inSpace) {
        mainText->setString("SPACE - SAFE");
        mainText->setFillColor(sf::Color::White);
        mainText->setOutlineColor(sf::Color(100, 100, 100));
        subText->setString("Timers paused");
    }
    else if (timers->isInHillZone) {
        mainText->setString(hSS.str());
        mainText->setFillColor(sf::Color(50, 200, 50));
        subText->setString(cSS.str());
    }
    else {
        mainText->setString(cSS.str());
        mainText->setFillColor(sf::Color(100, 200, 255));
        subText->setString(hSS.str());
    }

    // Positionnement UI
    float w = static_cast<float>(window->getSize().x);
    sf::FloatRect mB = mainText->getLocalBounds();
    mainText->setPosition({ (w / 2.f) - (mB.size.x / 2.f), 20.f });
    sf::FloatRect sB = subText->getLocalBounds();
    subText->setPosition({ (w / 2.f) - (sB.size.x / 2.f), 105.f });

    sf::View currentView = window->getView();
    window->setView(window->getDefaultView());
    window->draw(*mainText);
    window->draw(*subText);
    window->setView(currentView);
}