#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Settings.h"
#include "NetPackets.h"
#include "Scene.h"

class World;

class Client
{
public:
	Client(World*& world);

	// Called every frame from Engine::Run.
	void Run();

	// Attempt to connect to the server using the IP/port from World's UI inputs.
	void AttemptJoin();

	// Send DISCONNECT to the server and unbind the socket.
	void Disconnect();

	bool IsConnected() const { return connected; }

private:
	// --- Receive ---
	bool ReceiveOne();
	void DrainReceive();

	// --- Packet handlers ---
	void HandleNewConnection(sf::Packet& p);
	void HandleStartGame(sf::Packet& p);
	void HandlePlayerData(sf::Packet& p);
	void HandleDisconnect(sf::Packet& p);

	// --- Send ---
	void SendPlayerData();
	void SendStringMessage(const std::string& message);

	// --- Network state ---
	sf::UdpSocket socket;
	sf::IpAddress serverIp = sf::IpAddress::Any;
	unsigned short serverPort = 0;
	sf::IpAddress localAddress = sf::IpAddress::Any;
	unsigned short localPort = 0;
	bool connected = false;

	World* world = nullptr;
};
