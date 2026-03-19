#include "World.h"
#include <iostream>
#include "Scene.h"
#include "Engine.h"
#include "SceneModule.h"

World::World() :
	userPort(font),
	serverIp(font),
	serverPort(font),
	userPortDisplay(font),
	serverPortDisplay(font),
	serverIpDisplay(font),
	joinButtonText(font),
	hostButtonText(font),
	startServerHostText(font),
	goBackText(font),
	serverIpText(font),
	hostMenuInfo(font),
    startGameText(font),
    waitText(font),
    multiText(font)
{

	state = GameState::MAIN_MENU;

    SceneModule* sm = Engine::GetInstance()->GetModuleManager()->GetModule<SceneModule>();
    if (sm) {
        currentScene = sm->GetCurrentScene();
    }

	if (!font.openFromFile("Assets/Fonts/Independent Modern 8x8.ttf")) {
		std::cout << "[WORLD] Failed to load font" << std::endl;
	}


    if (!titleTexture.loadFromFile("Assets/Titre.png")) {
        std::cerr << "Erreur: Impossible de charger Titre.png" << std::endl;
    }
   // --- TITRE ---
    if (!titleTexture.loadFromFile("Assets/Titre.png")) {
        std::cerr << "Erreur: Impossible de charger Titre.png" << std::endl;
    }
    titleSprite.setTexture(titleTexture, true);
    
    // On réduit beaucoup l'échelle (à ajuster selon tes goûts, ex: 0.3f = 30% de la taille de base)
    float titleScale = 0.3f; 
    titleSprite.setScale(sf::Vector2f{ titleScale, titleScale });
    
    // Centrage horizontal et position tout en haut
    titleSprite.setPosition(sf::Vector2f{ 1200.f / 2.f - (titleTexture.getSize().x * titleScale) / 2.f, 50.f });


    // --- TEXTE MULTIJOUEUR ---
    multiText.setFont(font);
    multiText.setString("MULTIJOUEUR");
    // On le positionne par rapport au bouton qu'on va centrer plus bas
    // (Les valeurs +X et +Y servent à le centrer grossièrement dans la boîte verte)
    multiText.setPosition(sf::Vector2f{ uiw.multiButton.getPosition().x + 60.f, uiw.multiButton.getPosition().y + 15.f });

	userPort.setStyle(sf::Text::Bold);
	userPort.setCharacterSize(20);
	userPort.setString("Enter client port : ");
	userPort.setPosition(sf::Vector2f{ uiw.userPortBox.getPosition().x, 200 });


	serverIp.setStyle(sf::Text::Bold);
	serverIp.setCharacterSize(20);
	serverIp.setString("Enter server IP: ");
	serverIp.setPosition(sf::Vector2f{ uiw.serverIpBox.getPosition().x, 300 });

	serverPort.setStyle(sf::Text::Bold);
	serverPort.setCharacterSize(20);
	serverPort.setString("Enter server port: ");
	serverPort.setPosition(sf::Vector2f{ uiw.serverPortBox.getPosition().x, 400 });


	userPortDisplay.setStyle(sf::Text::Bold);
	userPortDisplay.setCharacterSize(20);
	userPortDisplay.setPosition(uiw.userPortBox.getPosition());

	serverPortDisplay.setStyle(sf::Text::Bold);
	serverPortDisplay.setCharacterSize(20);
	serverPortDisplay.setPosition(uiw.serverPortBox.getPosition());

	serverIpDisplay.setStyle(sf::Text::Bold);
	serverIpDisplay.setCharacterSize(20);
	serverIpDisplay.setPosition(uiw.serverIpBox.getPosition());

	joinButtonText.setStyle(sf::Text::Bold);
	joinButtonText.setCharacterSize(20);
	joinButtonText.setPosition(sf::Vector2f{ uiw.joinButton.getPosition().x + 22, uiw.joinButton.getPosition().y + 12 });
	joinButtonText.setString("JOIN");

	hostButtonText.setCharacterSize(20);
	hostButtonText.setPosition(sf::Vector2f{ uiw.hostButton.getPosition().x + 22, uiw.hostButton.getPosition().y + 12 });
	hostButtonText.setString("HOST");

	startServerHostText.setStyle(sf::Text::Bold);
	startServerHostText.setCharacterSize(20);
	startServerHostText.setString("START SERVER");
	startServerHostText.setPosition(sf::Vector2f{ uiw.startServerHostButton.getPosition().x + 22, uiw.startServerHostButton.getPosition().y + 12 });

	goBackText.setStyle(sf::Text::Bold);
	goBackText.setCharacterSize(20);
	goBackText.setString("BACK");
	goBackText.setPosition(sf::Vector2f{ uiw.goBackButton.getPosition().x + 22, uiw.goBackButton.getPosition().y + 12 });

	serverIpText.setStyle(sf::Text::Bold);
	serverIpText.setCharacterSize(40);
	serverIpText.setString("Server IP: Start server to reveal");
	serverIpText.setPosition(sf::Vector2f{ 350, 315 });

	hostMenuInfo.setStyle(sf::Text::Bold);
	hostMenuInfo.setCharacterSize(20);
	hostMenuInfo.setPosition(sf::Vector2f{ 275, 400 });
	hostMenuInfo.setString("Le serveur est en cours d'�xecution. Fermez la fen�tre pour fermer le serveur");

    waitText.setStyle(sf::Text::Bold);
    waitText.setCharacterSize(30);
    waitText.setPosition(sf::Vector2f{ 200.f, 300.f });
    waitText.setString("En attente du serveur");

    // --- TEXTE START GAME (Menu HOSTING) ---
    startGameText.setString("START GAME");
    startGameText.setCharacterSize(25); // Tu peux ajuster la taille

    // On récupère les dimensions du texte
    sf::FloatRect startBounds = startGameText.getLocalBounds();

    // On place l'origine au centre du texte
    startGameText.setOrigin(sf::Vector2f{
        startBounds.position.x + startBounds.size.x / 2.0f,
        startBounds.position.y + startBounds.size.y / 2.0f
        });

    // On place le texte exactement au centre du startGameButton
    startGameText.setPosition(sf::Vector2f{
        uiw.startGameButton.getPosition().x + uiw.startGameButton.getSize().x / 2.0f,
        uiw.startGameButton.getPosition().y + uiw.startGameButton.getSize().y / 2.0f
        });
}

void World::update(float dt) {
    switch (state) {
    case GameState::MAIN_MENU:
        if (uiw.goToMultiMenu) {
            uiw.goToMultiMenu = false;
            state = GameState::MULTIPLAYER_MENU;
        }
        else if (uiw.startSolo) {
            uiw.startSolo = false;
            state = GameState::PLAYING;
        }
        break;
    case GameState::MULTIPLAYER_MENU:
        if (uiw.goToHostScreen) {
            uiw.goToHostScreen = false;
            state = GameState::HOST;
        }
        break;
    case GameState::PLAYING:
        isFinished = true;
        break;
    case GameState::HOST:
        if (uiw.goBackToMain) {
            uiw.goBackToMain = false;
            state = GameState::MAIN_MENU;
        }
        if (uiw.attemptStartServer) {
            uiw.attemptStartServer = false;
            server.serverPort = static_cast<unsigned short>(std::stoul(serverPortInput));
            if (server.Init()) {
                serverPortDisplay.setString("Server Port : " + serverPortInput);
                serverIpText.setString("Server IP: " + server.GetIp().toString());
                state = GameState::HOSTING;
            }
        }
        break;
    case GameState::HOSTING:
        server.Run();
        if (uiw.attemptStartGame) {
            uiw.attemptStartGame = false;
            server.BroadcastGame();
            hosting = true;
            isFinished = true;
        }
        break;
    default:
        break;
    }
}


void World::render(sf::RenderWindow* window) {
    switch (state) {
    case GameState::MAIN_MENU:
        window->draw(titleSprite);
        window->draw(uiw.playButtonSprite);
        window->draw(uiw.multiButton);
        window->draw(multiText);

        uiw.update(window, state);
        break;
    case GameState::MULTIPLAYER_MENU:
        window->draw(userPort);
        window->draw(serverIp);
        window->draw(serverPort);

        uiw.update(window, state);

        uiw.draw(window);

        window->draw(userPortDisplay);
        window->draw(serverPortDisplay);
        window->draw(serverIpDisplay);
        window->draw(joinButtonText);
        window->draw(hostButtonText);

        break;
    case GameState::HOST:
        uiw.update(window, state);
        window->draw(uiw.startServerHostButton);
        window->draw(uiw.serverPortBox);
        window->draw(uiw.goBackButton);

        window->draw(startServerHostText);
        window->draw(serverIpText);
        window->draw(serverPort);
        window->draw(goBackText);
        window->draw(serverPortDisplay);
        break;
    case GameState::HOSTING:
        uiw.update(window, state);

        window->draw(uiw.startGameButton);
        window->draw(serverIpText);
        window->draw(serverPortDisplay);
        window->draw(hostMenuInfo);
        window->draw(startGameText);
        break;
    case GameState::WATINGFORHOST:
        uiw.update(window, state);
        window->draw(waitText);
        break;
    default:
        break;
    }
}

void World::processEvents(sf::RenderWindow* window) {
    while (const std::optional event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            isFinished = true;
        }
        else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
            if (textEntered->unicode < 256) {
                switch (uiw.currentSelected) {
                case UserInputWindow::SelectedBox::userPortSelect:
                    if (textEntered->unicode == 8 && !userPortInput.empty()) {
                        userPortInput.pop_back();
                    }
                    else if (textEntered->unicode != 8) {
                        userPortInput += static_cast<char>(textEntered->unicode);
                    }
                    userPortDisplay.setString(userPortInput);
                    break;
                case UserInputWindow::SelectedBox::serverIPSelect:
                    if (textEntered->unicode == 8 && !serverIPInput.empty()) {
                        serverIPInput.pop_back();
                    }
                    else if (textEntered->unicode != 8) {
                        serverIPInput += static_cast<char>(textEntered->unicode);
                    }
                    serverIpDisplay.setString(serverIPInput);
                    break;
                case UserInputWindow::SelectedBox::serverPortSelect:
                    if (textEntered->unicode == 8 && !serverPortInput.empty()) {
                        serverPortInput.pop_back();
                    }
                    else if (textEntered->unicode != 8) {
                        serverPortInput += static_cast<char>(textEntered->unicode);
                    }
                    serverPortDisplay.setString(serverPortInput);
                    break;
                case UserInputWindow::SelectedBox::none:
                    break;
                }
            }
        }
    }
}

void World::StartGame()
{
    Engine* engine = Engine::GetInstance();
    SceneModule* sm = engine->GetModuleManager()->GetModule<SceneModule>();

    currentScene = sm->GetCurrentScene();

    if (!currentScene)
        return;

    auto players = currentScene->GetGameObjectsWithName("Player");

    if (!players.empty())
    {
        playerContext.player = players[0];
        std::cout << "[WORLD] Player found\n";
    }
    else
    {
        std::cout << "[WORLD] Player not found in scene\n";
    }
}


World::UserInputWindow::UserInputWindow() {
	currentSelected = SelectedBox::none;

	userPortBox.setSize(sf::Vector2f{ 180, 30 });
	userPortBox.setPosition(sf::Vector2f{ 510, 240 });
	userPortBox.setFillColor(sf::Color::White);

	serverIpBox.setSize(sf::Vector2f{ 180, 30 });
	serverIpBox.setPosition(sf::Vector2f{ 510, 340 });
	serverIpBox.setFillColor(sf::Color::White);

	serverPortBox.setSize(sf::Vector2f{ 180, 30 });
	serverPortBox.setPosition(sf::Vector2f{ 510, 440 });
	serverPortBox.setFillColor(sf::Color::White);

	joinButton.setSize(sf::Vector2f{ 100, 50 });
	joinButton.setPosition(sf::Vector2f{ 550, 500 });
	joinButton.setFillColor(sf::Color::Green);

	hostButton.setSize(sf::Vector2f{ 100, 50 });
	hostButton.setPosition(sf::Vector2f{ 100, 100 });
	hostButton.setFillColor(darkGray);

	startServerHostButton.setSize(sf::Vector2f{ 200, 50 });
	startServerHostButton.setPosition(sf::Vector2f{ 500, 575 });
	startServerHostButton.setFillColor(darkGray);

	goBackButton.setSize(sf::Vector2f{ 100, 50 });
	goBackButton.setPosition(sf::Vector2f{ 100, 200 });
	goBackButton.setFillColor(darkGreen);

    // --- BOUTON START GAME (Menu HOSTING) ---
    startGameButton.setSize(sf::Vector2f{ 300.f, 60.f });
    // On le centre horizontalement, et on le place en bas (ex: Y = 600)
    startGameButton.setPosition(sf::Vector2f{ 1200.f / 2.f - 150.f, 620.f });
    startGameButton.setFillColor(darkGreen);

    // --- BOUTON PLAY ---
    if (!playButtonTexture.loadFromFile("Assets/PlayButton.png")) {
        std::cerr << "Erreur: Impossible de charger PlayButton.png" << std::endl;
    }
    playButtonSprite.setTexture(playButtonTexture, true);

    float playScale = 0.25f;
    playButtonSprite.setScale(sf::Vector2f{ playScale, playScale });

    // 1. On place l'origine au centre exact de la texture
    playButtonSprite.setOrigin(sf::Vector2f{
        playButtonTexture.getSize().x / 2.0f,
        playButtonTexture.getSize().y / 2.0f
        });

    // 2. On le pivote (angle négatif = sens anti-horaire)
    // Note : Puisque tu es sur SFML 3, on précise qu'il s'agit de degrés avec sf::degrees()
    playButtonSprite.setRotation(sf::degrees(20.f));

    

    // 3. On le positionne. 
    // Magie : comme l'origine est au centre, la position X est simplement la moitié de l'écran (600) !
    playButtonSprite.setPosition(sf::Vector2f{ 1200.f / 2.f, 470.f });


    // --- BOUTON MULTI (Boîte verte) ---
    multiButton.setSize(sf::Vector2f{ 300.f, 60.f });

    // On centre la boîte horizontalement (1200/2 - 300/2 = 450)
    // Et on la met bien en dessous du bouton Play (Y = 550)
    multiButton.setPosition(sf::Vector2f{ 1200.f / 2.f - 150.f, 550.f });
    multiButton.setFillColor(darkGreen);
}

void World::UserInputWindow::draw(sf::RenderWindow* w) {
	w->draw(userPortBox);
	w->draw(serverIpBox);
	w->draw(serverPortBox);
	w->draw(joinButton);
	w->draw(hostButton);
}

void World::UserInputWindow::update(sf::RenderWindow* w, GameState g) {
    sf::Vector2i mousePosition{ sf::Mouse::getPosition(*w) };
    switch (g) {
    case GameState::MAIN_MENU:
        // Logique bouton SOLO
        if (playButtonSprite.getGlobalBounds().contains(sf::Vector2f{ static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) })) {
            playButtonSprite.setColor(lightGreen);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                startSolo = true;
            }
        }
        else playButtonSprite.setColor(darkGreen);

        // Logique bouton MULTI
        if (multiButton.getGlobalBounds().contains(sf::Vector2f{static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)})) {
            multiButton.setFillColor(lightGreen);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                goToMultiMenu = true;
            }
        }
        else multiButton.setFillColor(darkGreen);

        break;
    case GameState::MULTIPLAYER_MENU:

        if (userPortBox.getGlobalBounds().contains(sf::Vector2f{ static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y) })) {
            userPortBox.setFillColor(gray);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                currentSelected = SelectedBox::userPortSelect;
                serverIpBox.setFillColor(darkGray);
                serverPortBox.setFillColor(darkGray);
            }
        }
        else {
            userPortBox.setFillColor(darkGray);
        }

        if (serverIpBox.getGlobalBounds().contains(sf::Vector2f{ static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y) })) {
            serverIpBox.setFillColor(gray);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                currentSelected = SelectedBox::serverIPSelect;
                serverPortBox.setFillColor(darkGray);
                userPortBox.setFillColor(darkGray);
            }
        }
        else {
            serverIpBox.setFillColor(darkGray);
        }

        if (serverPortBox.getGlobalBounds().contains(sf::Vector2f{ static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y) })) {
            serverPortBox.setFillColor(gray);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                currentSelected = SelectedBox::serverPortSelect;
                userPortBox.setFillColor(darkGray);
                serverIpBox.setFillColor(darkGray);
            }
        }
        else {
            serverPortBox.setFillColor(darkGray);
        }

        switch (currentSelected) {
        case SelectedBox::userPortSelect:
            userPortBox.setFillColor(lightGray);
            break;
        case SelectedBox::serverIPSelect:
            serverIpBox.setFillColor(lightGray);
            break;
        case SelectedBox::serverPortSelect:
            serverPortBox.setFillColor(lightGray);
            break;
        case SelectedBox::none:
            break;
        }

        if (joinButton.getGlobalBounds().contains(sf::Vector2f{ static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y) })) {
            joinButton.setFillColor(lightGreen);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                attemptJoin = true;
                currentSelected = none;
            }
        }
        else {
            joinButton.setFillColor(darkGreen);
        }

        if (hostButton.getGlobalBounds().contains(sf::Vector2f{ static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y) })) {
            hostButton.setFillColor(lightGray);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                goToHostScreen = true;
                currentSelected = none;
            }
        }
        else {
            hostButton.setFillColor(darkGray);
        }

        break;
    case GameState::HOST:

        if (startServerHostButton.getGlobalBounds().contains(sf::Vector2f{ static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y) })) {
            startServerHostButton.setFillColor(lightGray);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                attemptStartServer = true;
                currentSelected = none;
            }
        }
        else {
            startServerHostButton.setFillColor(darkGray);
        }

        if (goBackButton.getGlobalBounds().contains(sf::Vector2f{ static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y) })) {
            goBackButton.setFillColor(lightGreen);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                goBackToMain = true;
                currentSelected = none;
            }
        }
        else {
            goBackButton.setFillColor(darkGreen);
        }

        if (serverPortBox.getGlobalBounds().contains(sf::Vector2f{ static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y) })) {
            serverPortBox.setFillColor(gray);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                currentSelected = SelectedBox::serverPortSelect;
                userPortBox.setFillColor(darkGray);
                serverIpBox.setFillColor(darkGray);
            }
        }
        else {
            serverPortBox.setFillColor(darkGray);
        }

        if (currentSelected == SelectedBox::serverPortSelect) {
            serverPortBox.setFillColor(lightGray);
        }

        break;
    case GameState::HOSTING:
        // On utilise startGameButton au lieu de playButtonSprite
        if (startGameButton.getGlobalBounds().contains(sf::Vector2f{ static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y) })) {
            startGameButton.setFillColor(lightGreen);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                attemptStartGame = true;
                currentSelected = none;
            }
        }
        else {
            startGameButton.setFillColor(darkGreen);
        }
        break;
    default:
        break;
    }    
}