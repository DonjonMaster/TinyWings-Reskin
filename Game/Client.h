#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "settings.h"

class Client
{
public :
	Client();

	void ReceiveData();
	void SendData();
	void disconnect();
	// Ajouter des actions du joueur si nécessaire

	// Lancer le jeu (Player et monde)
	void run();

	void AttemptJoin();

private:
	sf::IpAddress serverIp = sf::IpAddress::Any;
	unsigned short serverPort;

	sf::IpAddress adress;
	unsigned short port;
	sf::UdpSocket socket;

	// Ajouter les données à envoyer

	bool connected{ false };
};

