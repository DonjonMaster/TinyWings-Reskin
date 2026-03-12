#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <unordered_map>

#include <string>

#include "settings.h"

struct Connection {
	sf::IpAddress address{ sf::IpAddress::Any };
	unsigned short port{ 0 };

	int playerNumber{ 0 };

	// Ajouter information des joueurs et de leurs positions dans le monde (données à envoyer / recevoir)
	sf::Vector2f position{ 0, 0 };
};

class Server
{
public:
	Server();

	void Run();
	bool Init();

	void ReceiveData();
	void SendData(std::string& sender, sf::Packet& p);
	void SendInitialData(Connection& c);
	void SendDataToEveryone(sf::Packet& p);

	void BroadcastGame();

	sf::IpAddress GetIp();
	unsigned short serverPort;

private:
	sf::IpAddress serverIp = sf::IpAddress::Any;
	sf::UdpSocket serverSocket;

	std::unordered_map<std::string, Connection> connections;
};

