#include "Client.h"
#include "World.h"

Client::Client(World*& world) : localAddress(sf::IpAddress::Any) {
	this->world = world;
	auto localIp = sf::IpAddress::getLocalAddress();
	if (localIp) {
		localAddress = *localIp;
	}
}

// ============================================================
// Main loop — called once per frame from Engine::Run.
// ============================================================
void Client::Run() {
	// Handle a join request from the UI.
	if (world->uiw.attemptJoin) {
		AttemptJoin();
		world->uiw.attemptJoin = false;
	}

	// Receive all pending packets.
	if (world->state == GameState::WATINGFORHOST || world->state == GameState::PLAYING) {
		DrainReceive();
	}

	// Send our position to the server.
	if (world->state == GameState::PLAYING && connected) {
		SendPlayerData();
	}
}

// ============================================================
// Receive
// ============================================================

void Client::DrainReceive() {
	while (ReceiveOne());
}

bool Client::ReceiveOne() {
	sf::Packet p;
	std::optional<sf::IpAddress> sIp;
	unsigned short sP;

	sf::Socket::Status status = socket.receive(p, sIp, sP);

	if (status == sf::Socket::Status::NotReady) return false;
	if (status == sf::Socket::Status::Error) {
		//std::cout << "[CLIENT] Socket error while receiving" << std::endl;
		return false;
	}
	if (status != sf::Socket::Status::Done) return false;

	Settings::PacketTypes type = ReadHeader(p);

	switch (type) {
	case Settings::PacketTypes::NEW_CONNECTION:
		HandleNewConnection(p);
		break;
	case Settings::PacketTypes::START_GAME:
		HandleStartGame(p);
		break;
	case Settings::PacketTypes::PLAYER_DATA:
		HandlePlayerData(p);
		break;
	case Settings::PacketTypes::DISCONNECT:
		HandleDisconnect(p);
		break;
	default:
		std::cout << "[CLIENT][RECV] Unknown packet type: " << type << std::endl;
		break;
	}

	return true;
}

// ============================================================
// Packet handlers
// ============================================================

void Client::HandleNewConnection(sf::Packet& p) {
	ConnectionMsg msg;
	if (p >> msg) {
		std::cout << "[CLIENT][RECV] Connected as Player " << msg.playerNumber
			<< " (ID: " << msg.playerKey << ")" << std::endl;
		SendStringMessage("Hello from client!");
	}
}

void Client::HandleStartGame(sf::Packet& p) {
	std::cout << "[CLIENT][RECV] Server started the game" << std::endl;
	world->StartGame();
	world->state = GameState::PLAYING;
}

void Client::HandlePlayerData(sf::Packet& p) {
	ServerPlayerDataMsg msg;
	if (p >> msg) {
		std::cout << "[CLIENT][RECV] PLAYER_DATA from " << msg.senderKey
			<< " -> (" << msg.position.x << ", " << msg.position.y << ")" << std::endl;

		// Update the ghost (remote player) position in the scene.
		if (world->currentScene) {
			world->currentScene->UpdatePos(msg.position);
		}
	}
}

void Client::HandleDisconnect(sf::Packet& p) {
	DisconnectMsg msg;
	if (p >> msg) {
		std::cout << "[CLIENT][RECV] Player " << msg.playerKey << " disconnected" << std::endl;
	}
}

// ============================================================
// Send
// ============================================================

void Client::SendPlayerData() {
	if (!connected || !world || !world->playerContext.player) return;

	auto& transform = world->playerContext.player->GetTransform();

	sf::Packet p;
	ClientPlayerDataMsg msg{ transform.pos };
	p << msg;

	if (socket.send(p, serverIp, serverPort) != sf::Socket::Status::Done) {
		std::cout << "[CLIENT][SEND] Error sending player data" << std::endl;
	}
}

void Client::SendStringMessage(const std::string& message) {
	if (!connected) return;

	sf::Packet p;
	StringMsg msg{ message };
	p << msg;

	if (socket.send(p, serverIp, serverPort) != sf::Socket::Status::Done) {
		std::cout << "[CLIENT][SEND] Error sending string message" << std::endl;
	}
}

// ============================================================
// Connection management
// ============================================================

void Client::AttemptJoin() {
	if (connected) {
		std::cout << "[CLIENT] Already connected, ignoring join attempt" << std::endl;
		return;
	}

	auto resolvedIp = sf::IpAddress::resolve(world->serverIPInput);
	if (!resolvedIp) {
		std::cout << "[CLIENT] Invalid IP address" << std::endl;
		return;
	}

	serverIp = *resolvedIp;
	serverPort = static_cast<unsigned short>(std::stoul(world->serverPortInput));
	localPort = static_cast<unsigned short>(std::stoul(world->userPortInput));

	if (socket.bind(localPort) != sf::Socket::Status::Done) {
		std::cout << "[CLIENT] Failed to bind socket to port " << localPort << std::endl;
		return;
	}
	socket.setBlocking(false);

	// Send connection request — header only, no payload.
	// Server identifies us by the UDP source ip:port.
	sf::Packet p;
	p << Settings::PacketTypes::NEW_CONNECTION;

	if (socket.send(p, serverIp, serverPort) != sf::Socket::Status::Done) {
		std::cout << "[CLIENT] Failed to send connection request" << std::endl;
		socket.unbind();
		return;
	}

	std::cout << "[CLIENT] Connection request sent to " << serverIp << ":" << serverPort << std::endl;
	connected = true;
	world->state = GameState::WATINGFORHOST;
}

void Client::Disconnect() {
	if (!connected) return;

	std::string key = localAddress.toString() + ":" + std::to_string(localPort);
	std::cout << "[CLIENT] Disconnecting: " << key << std::endl;

	sf::Packet p;
	DisconnectMsg msg{ key };
	p << msg;

	if (socket.send(p, serverIp, serverPort) != sf::Socket::Status::Done) {
		std::cout << "[CLIENT][SEND] Error sending DISCONNECT" << std::endl;
	}

	socket.unbind();
	connected = false;
}
