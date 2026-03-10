#pragma once

#include <SFML/Graphics.hpp>

#include "Server.h"

enum GameState {
    MAIN_MENU = 0,
    PLAYING,
    HOST,
    HOSTING
};

class World
{
public:
	World();
	~World();

	void update(float dt);
	void render();
	// Autres fonctions
	
    struct UserInputWindow {
        UserInputWindow();

        enum SelectedBox {
            userPortSelect = 0,
            serverIPSelect,
            serverPortSelect,
            none
        };

        SelectedBox currentSelected;

        sf::RectangleShape userPortBox;
        sf::RectangleShape serverIpBox;
        sf::RectangleShape serverPortBox;

        sf::RectangleShape joinButton;
        sf::RectangleShape hostButton;
        sf::RectangleShape startServerHostButton;
        sf::RectangleShape goBackButton;

        sf::Color lightGray{ 200, 200, 200 };
        sf::Color gray{ 152, 156, 155 };
        sf::Color darkGray{ 125, 125, 125 };
        sf::Color darkGreen{ 78, 153, 63 };
        sf::Color lightGreen{ 109, 204, 90 };

        bool attemptJoin{ false };
        bool goToHostScreen{ false };
        bool goBackToMain{ false };
        bool attemptStartServer{ false };


    public:
        void update(sf::RenderWindow&, GameState);

        void draw(sf::RenderWindow&);
    };

    sf::RenderWindow window;
    sf::Font font;

    sf::Text gameName;
    sf::Text userPort;
    sf::Text serverIp;
    sf::Text serverPort;

    UserInputWindow uiw;
    sf::Text userPortDisplay;
    sf::Text serverIpDisplay;
    sf::Text serverPortDisplay;
    sf::Text joinButtonText;
    sf::Text hostButtonText;
    sf::Text startServerHostText;
    sf::Text goBackText;
    sf::Text serverIpText;
    sf::Text hostMenuInfo;
    std::string userPortInput{};
    std::string serverIPInput{};
    std::string serverPortInput{};

    Server server{};

    GameState state;

    bool RoomFull{ false };
};

