#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "PlayerContext.h"
#include "Server.h"

class GameObject;
class Scene;

enum class GameState {
    MAIN_MENU = 0,
    MULTIPLAYER_MENU,
    PLAYING,
    HOST,
    HOSTING,
    WATINGFORHOST
};

class World
{
public:
    Scene* currentScene = nullptr;
    PlayerContext playerContext;

    bool hosting = false;

    sf::Texture titleTexture;
    sf::Sprite titleSprite{ titleTexture };

    // sprite et texture pour l'ecran titre etc
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite{ menuBackgroundTexture };

	World();
	~World();

	void update(float dt);
	void render(sf::RenderWindow* window);
    void processEvents(sf::RenderWindow* window);
    void StartGame();
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
        sf::RectangleShape startGameButton;
        
        sf::Texture playButtonTexture;
        sf::Sprite playButtonSprite{ playButtonTexture };

        sf::Texture multiButtonTexture;
        sf::Sprite multiButtonSprite{ multiButtonTexture };

        sf::Color lightGray{ 200, 200, 200 };
        sf::Color gray{ 152, 156, 155 };
        sf::Color darkGray{ 125, 125, 125 };
        sf::Color darkGreen{ 78, 153, 63 };
        sf::Color lightGreen{ 109, 204, 90 };

        bool attemptJoin{ false };
        bool goToHostScreen{ false };
        bool goBackToMain{ false };
        bool attemptStartServer{ false };
        bool attemptStartGame{ false };


        bool goToMultiMenu{ false };
        bool startSolo{ false };

    public:
        void update(sf::RenderWindow*, GameState);

        void draw(sf::RenderWindow*);
    };

    sf::Font font;

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
    sf::Text startGameText;
    sf::Text waitText;

    sf::Text multiText;

    std::string userPortInput{};
    std::string serverIPInput{};
    std::string serverPortInput{};

    Server server{};

    GameState state;

    bool isFinished{ false };
};

