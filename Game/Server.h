#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <unordered_map>
#include <string>

#include "Settings.h"
#include "NetPackets.h"

// Represents a connected client as seen by the server.
struct Connection {
	sf::IpAddress address{ sf::IpAddress::Any };
	unsigned short port{ 0 };
	int playerNumber{ 0 };
	sf::Vector2f position{ 0, 0 };
	float score{ 0.f };
};

class Server
{
public:
	Server();

	// Convenience: DrainReceive + FlushSends in one call (used in lobby).
	void Run();

	// Bind the server socket. Returns false on failure.
	bool Init();

	// --- Frame-split API (used in-game by Engine) ---
	void DrainReceive();
	void FlushSends();

	// Notify all clients that the game is starting.
	void BroadcastGame();

	sf::IpAddress GetIp() const;
	unsigned short serverPort = 0;

private:
	// --- Packet handlers (called from DrainReceive) ---
	void HandleNewConnection(sf::Packet& packet, const sf::IpAddress& senderIp,
		unsigned short senderPort, const std::string& senderKey);
	void HandlePlayerData(sf::Packet& packet, const std::string& senderKey);
	void HandleDisconnect(sf::Packet& packet, const std::string& senderKey);
	void HandleStringMessage(sf::Packet& packet, const std::string& senderKey);

	// --- Send helpers ---
	void SendToOthers(const std::string& sender, sf::Packet& p);
	void SendInitialData(const std::string& newKey, const Connection& c);
	void SendToEveryone(sf::Packet& p);

	sf::IpAddress serverIp = sf::IpAddress::Any;
	sf::UdpSocket serverSocket;

	std::unordered_map<std::string, Connection> connections;
	std::unordered_map<std::string, sf::Vector2f> pendingPositions;
};
