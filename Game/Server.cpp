#include "Server.h"

Server::Server() : serverIp(sf::IpAddress::Any) {
	serverSocket.setBlocking(false);
	auto localIp = sf::IpAddress::getLocalAddress();
	if (localIp) {
		serverIp = *localIp;
	}
}

void Server::Run() {
	DrainReceive();
	FlushSends();
}

bool Server::Init() {
	if (serverSocket.bind(serverPort) != sf::Socket::Status::Done) {
		std::cout << "[SERVER] Failed to bind socket to port " << serverPort << std::endl;
		return false;
	}
	serverSocket.setBlocking(false);
	std::cout << "[SERVER] Socket bound to port " << serverPort << std::endl;
	return true;
}

// ============================================================
// Receive phase — drains every pending packet from the socket.
// ============================================================
void Server::DrainReceive() {
	sf::Packet packet;
	std::optional<sf::IpAddress> senderIp;
	unsigned short senderPort;

	while (true) {
		packet.clear();
		sf::Socket::Status status = serverSocket.receive(packet, senderIp, senderPort);

		if (status == sf::Socket::Status::NotReady) break;

		if (status != sf::Socket::Status::Done) {
			std::cout << "[SERVER][RECV] Socket error: status=" << static_cast<int>(status) << std::endl;
			break;
		}

		if (!senderIp) {
			std::cout << "[SERVER][RECV] Invalid packet (no sender IP)" << std::endl;
			continue;
		}

		Settings::PacketTypes type = ReadHeader(packet);
		std::string senderKey = senderIp->toString() + ":" + std::to_string(senderPort);

		switch (type) {
		case Settings::PacketTypes::NEW_CONNECTION:
			HandleNewConnection(packet, *senderIp, senderPort, senderKey);
			break;
		case Settings::PacketTypes::PLAYER_DATA:
			HandlePlayerData(packet, senderKey);
			break;
		case Settings::PacketTypes::DISCONNECT:
			HandleDisconnect(packet, senderKey);
			break;
		case Settings::PacketTypes::STRING_MESSAGE:
			HandleStringMessage(packet, senderKey);
			break;
		default:
			std::cout << "[SERVER][RECV] Unknown packet type: " << type
				<< " from " << senderKey << std::endl;
			break;
		}
	}
}

// ============================================================
// Send phase — relays all queued positions to other clients.
// ============================================================
void Server::FlushSends() {
	if (pendingPositions.empty()) return;

	for (const auto& [senderKey, pos] : pendingPositions) {
		sf::Packet relay;
		ServerPlayerDataMsg msg{ senderKey, pos };
		relay << msg;

		int sentCount = 0;
		for (const auto& [key, val] : connections) {
			if (key == senderKey) continue;
			if (serverSocket.send(relay, val.address, val.port) == sf::Socket::Status::Done) {
				sentCount++;
			} else {
				std::cout << "[SERVER][SEND] Failed to relay PLAYER_DATA to " << key << std::endl;
			}
		}
		std::cout << "[SERVER][SEND] Relayed position from " << senderKey
			<< " to " << sentCount << " client(s)" << std::endl;
	}

	pendingPositions.clear();
}

// ============================================================
// Packet handlers
// ============================================================

void Server::HandleNewConnection(sf::Packet& packet, const sf::IpAddress& senderIp,
	unsigned short senderPort, const std::string& senderKey)
{
	// Already connected? Ignore duplicate.
	if (connections.count(senderKey)) {
		std::cout << "[SERVER][RECV] Duplicate NEW_CONNECTION from " << senderKey << " - ignored" << std::endl;
		return;
	}

	if (connections.size() >= 2) {
		std::cout << "[SERVER][RECV] NEW_CONNECTION from " << senderKey << " rejected: lobby full" << std::endl;
		return;
	}

	Connection c;
	c.address = senderIp;
	c.port = senderPort;
	c.playerNumber = static_cast<int>(connections.size()) + 1;
	connections[senderKey] = c;

	std::cout << "[SERVER][RECV] NEW_CONNECTION from " << senderKey
		<< " -> Player " << c.playerNumber << std::endl;
	std::cout << "[SERVER] Total connections: " << connections.size() << std::endl;

	// Confirm the connection to the new client.
	sf::Packet reply;
	ConnectionMsg confirm{ senderKey, c.playerNumber };
	reply << confirm;

	if (serverSocket.send(reply, senderIp, senderPort) != sf::Socket::Status::Done) {
		std::cout << "[SERVER][SEND] Failed to confirm NEW_CONNECTION to " << senderKey << std::endl;
	}

	// Send info about players already in the lobby.
	SendInitialData(senderKey, connections[senderKey]);
}

void Server::HandlePlayerData(sf::Packet& packet, const std::string& senderKey) {
	if (connections.count(senderKey) == 0) {
		std::cout << "[SERVER][RECV] PLAYER_DATA from unknown sender " << senderKey << " - ignored" << std::endl;
		return;
	}

	ClientPlayerDataMsg msg;
	if (packet >> msg) {
		connections[senderKey].position = msg.position;
		connections[senderKey].score = msg.score;

		ServerPlayerDataMsg outMsg{ senderKey, msg.position, msg.score };
		sf::Packet outPacket;
		outPacket << outMsg;

		//pendingPositions[senderKey] = msg.position;

		std::cout << "[SERVER][RECV] PLAYER_DATA from " << senderKey
			<< " -> (" << msg.position.x << ", " << msg.position.y << ", " << msg.score << ")" << std::endl;

		SendToOthers(senderKey, outPacket);
	} else {
		std::cout << "[SERVER][RECV] PLAYER_DATA from " << senderKey
			<< ": failed to extract position" << ", " << msg.score << std::endl;
	}
}

void Server::HandleDisconnect(sf::Packet& packet, const std::string& senderKey) {
	if (connections.count(senderKey) == 0) {
		std::cout << "[SERVER][RECV] DISCONNECT from unknown sender " << senderKey << " - ignored" << std::endl;
		return;
	}

	std::cout << "[SERVER][RECV] DISCONNECT from " << senderKey << std::endl;

	pendingPositions.erase(senderKey);

	// Notify other clients using a typed DisconnectMsg.
	sf::Packet notify;
	DisconnectMsg msg{ senderKey };
	notify << msg;
	SendToOthers(senderKey, notify);

	connections.erase(senderKey);
	std::cout << "[SERVER] Total connections: " << connections.size() << std::endl;
}

void Server::HandleStringMessage(sf::Packet& packet, const std::string& senderKey) {
	StringMsg msg;
	if (packet >> msg) {
		std::cout << "[SERVER][RECV] MESSAGE from " << senderKey << ": " << msg.message << std::endl;
	}
}

// ============================================================
// Send helpers
// ============================================================

void Server::SendToOthers(const std::string& sender, sf::Packet& p) {
	for (const auto& [key, val] : connections) {
		if (key == sender) continue;
		if (serverSocket.send(p, val.address, val.port) != sf::Socket::Status::Done) {
			std::cout << "[SERVER][SEND] Failed to send to " << key << std::endl;
		}
	}
}

void Server::SendInitialData(const std::string& newKey, const Connection& c) {
	for (const auto& [key, val] : connections) {
		if (key == newKey) continue;

		sf::Packet p;
		ConnectionMsg msg{ key, val.playerNumber };
		p << msg;

		if (serverSocket.send(p, c.address, c.port) == sf::Socket::Status::Done) {
			std::cout << "[SERVER][SEND] Sent initial data (Player " << val.playerNumber
				<< ") to " << newKey << std::endl;
		} else {
			std::cout << "[SERVER][SEND] Failed to send initial data (Player " << val.playerNumber
				<< ") to " << newKey << std::endl;
		}
	}
}

void Server::SendToEveryone(sf::Packet& p) {
	for (const auto& [key, val] : connections) {
		if (serverSocket.send(p, val.address, val.port) != sf::Socket::Status::Done) {
			std::cout << "[SERVER][SEND] Failed to broadcast to " << key << std::endl;
		}
	}
}

void Server::BroadcastGame() {
	sf::Packet p;
	StartGameMsg msg;
	p << msg;

	std::cout << "[SERVER][SEND] Broadcasting START_GAME to " << connections.size() << " client(s)" << std::endl;

	SendToEveryone(p);
}

sf::IpAddress Server::GetIp() const {
	return serverIp;
}
