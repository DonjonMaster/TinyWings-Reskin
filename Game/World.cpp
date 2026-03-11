#include "World.h"

World::World() :
	gameName(font),
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
	hostMenuInfo(font)
{

	state = MAIN_MENU;

    window.create(sf::VideoMode({ 1280, 720 }), "Tiny Wings");

	if (!font.openFromFile("Assets/Fonts/Independent Modern 8x8.ttf")) {
		std::cout << "Impossible de charger la police" << std::endl;
	}

	gameName.setStyle(sf::Text::Bold);
	gameName.setCharacterSize(50);
	gameName.setString("Tiny Wings");
	gameName.setPosition(sf::Vector2f{ 410, 50 });

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

	uiw = UserInputWindow();

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
	serverIpText.setString("Server IP: " + server.GetIp().toString());
	serverIpText.setPosition(sf::Vector2f{ 350, 315 });

	hostMenuInfo.setStyle(sf::Text::Bold);
	hostMenuInfo.setCharacterSize(20);
	hostMenuInfo.setPosition(sf::Vector2f{ 275, 400 });
	hostMenuInfo.setString("Le serveur est en cours d'éxecution. Fermez la fenêtre pour fermer le serveur");
}

void World::update(float dt) {
    switch (state) {
    case GameState::MAIN_MENU:
        if (uiw.goToHostScreen) {
            uiw.goToHostScreen = false;
            state = HOST;
        }
        break;
    case GameState::PLAYING:
        // Update du joueur
        break;
    case GameState::HOST:
        if (uiw.goBackToMain) {
            uiw.goBackToMain = false;
            state = MAIN_MENU;
        }
        if (uiw.attemptStartServer) {
            server.serverPort = std::stoul(serverPortInput);
            if (server.Init()) {
                serverPortDisplay.setString("Server Port : " + serverPortInput);
                state = HOSTING;
            }
        }
        break;
    case GameState::HOSTING:
        server.ReceiveData();
        break;
    default:
        break;
    }
}

void World::render() {
    window.clear();

    switch (state) {
    case GameState::MAIN_MENU:
        window.draw(gameName);

        window.draw(userPort);
        window.draw(serverIp);
        window.draw(serverPort);

        uiw.update(window, state);

        uiw.draw(window);

        window.draw(userPortDisplay);
        window.draw(serverPortDisplay);
        window.draw(serverIpDisplay);
        window.draw(joinButtonText);
        window.draw(hostButtonText);

        break;
    case GameState::HOST:
        uiw.update(window, state);
        window.draw(uiw.startServerHostButton);
        window.draw(uiw.serverPortBox);
        window.draw(uiw.goBackButton);

        window.draw(startServerHostText);
        window.draw(serverIpText);
        window.draw(serverPort);
        window.draw(goBackText);
        window.draw(serverPortDisplay);
        break;
    case GameState::HOSTING:
        window.draw(serverIpText);
        window.draw(serverPortDisplay);
        window.draw(hostMenuInfo);
        break;
    default:
        break;
    }

    window.display();
}

void World::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
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
}

void World::UserInputWindow::draw(sf::RenderWindow& w) {
	w.draw(userPortBox);
	w.draw(serverIpBox);
	w.draw(serverPortBox);
	w.draw(joinButton);
	w.draw(hostButton);
}

void World::UserInputWindow::update(sf::RenderWindow& w, GameState g) {
    sf::Vector2i mousePosition{ sf::Mouse::getPosition(w) };
    switch (g) {
    case MAIN_MENU:

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
    case HOST:

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
    default:
        break;
    }

}