#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <cstdint>

#include "Settings.h"

// ============================================================
// Typed network messages for the game.
//
// Each struct represents one kind of packet that travels over
// the network.  The matching operator<< / operator>> guarantee
// that fields are always written and read in the same order,
// eliminating the most common source of networking bugs.
//
// Usage (send side — e.g. client sending position):
//     ClientPlayerDataMsg msg{ {100.f, 200.f} };
//     sf::Packet p;
//     p << msg;                       // header is written automatically
//     socket.send(p, ip, port);
//
// Usage (receive side — e.g. client receiving relayed position):
//     Settings::PacketTypes type = ReadHeader(packet);
//     if (type == Settings::PLAYER_DATA) {
//         ServerPlayerDataMsg msg;
//         packet >> msg;              // extracts all fields
//     }
// ============================================================

// --- NEW_CONNECTION -----------------------------------------
// Sent by: client  -> server  (request, no payload needed)
//          server  -> client  (confirmation with key + player number)
//          server  -> client  (initial data about other players)
struct ConnectionMsg {
	std::string playerKey;      // "ip:port" identifier assigned by server
	std::int32_t playerNumber = 0;
};

inline sf::Packet& operator<<(sf::Packet& p, const ConnectionMsg& m) {
	return p << Settings::PacketTypes::NEW_CONNECTION
		<< m.playerKey << m.playerNumber;
}

inline sf::Packet& operator>>(sf::Packet& p, ConnectionMsg& m) {
	return p >> m.playerKey >> m.playerNumber;
}

// --- PLAYER_DATA (client -> server) -------------------------
// The client only sends its position.
// The server identifies the sender via the UDP source ip:port.
struct ClientPlayerDataMsg {
	sf::Vector2f position{ 0.f, 0.f };
	float score{ 0.f };
};

inline sf::Packet& operator<<(sf::Packet& p, const ClientPlayerDataMsg& m) {
	return p << Settings::PacketTypes::PLAYER_DATA
		<< m.position.x << m.position.y << m.score;
}

inline sf::Packet& operator>>(sf::Packet& p, ClientPlayerDataMsg& m) {
	return p >> m.position.x >> m.position.y >> m.score;
}

// --- PLAYER_DATA (server -> client) -------------------------
// The server relays the position along with the sender's key
// so the receiving client knows which remote player moved.
struct ServerPlayerDataMsg {
	std::string senderKey;
	sf::Vector2f position{ 0.f, 0.f };
	float score{ 0.f };
};

inline sf::Packet& operator<<(sf::Packet& p, const ServerPlayerDataMsg& m) {
	return p << Settings::PacketTypes::PLAYER_DATA
		<< m.senderKey << m.position.x << m.position.y << m.score;
}

inline sf::Packet& operator>>(sf::Packet& p, ServerPlayerDataMsg& m) {
	return p >> m.senderKey >> m.position.x >> m.position.y << m.score;
}

// --- DISCONNECT ---------------------------------------------
// Sent by: client  -> server  (own key so server can verify)
//          server  -> client  (notifying others who left)
struct DisconnectMsg {
	std::string playerKey;
};

inline sf::Packet& operator<<(sf::Packet& p, const DisconnectMsg& m) {
	return p << Settings::PacketTypes::DISCONNECT
		<< m.playerKey;
}

inline sf::Packet& operator>>(sf::Packet& p, DisconnectMsg& m) {
	return p >> m.playerKey;
}

// --- STRING_MESSAGE -----------------------------------------
// Debug / chat message.
struct StringMsg {
	std::string message;
};

inline sf::Packet& operator<<(sf::Packet& p, const StringMsg& m) {
	return p << Settings::PacketTypes::STRING_MESSAGE
		<< m.message;
}

inline sf::Packet& operator>>(sf::Packet& p, StringMsg& m) {
	return p >> m.message;
}

// --- START_GAME ---------------------------------------------
// No payload — the header alone is the message.
struct StartGameMsg {};

inline sf::Packet& operator<<(sf::Packet& p, const StartGameMsg&) {
	return p << Settings::PacketTypes::START_GAME;
}

inline sf::Packet& operator>>(sf::Packet& p, StartGameMsg&) {
	return p; // nothing to extract
}

// ============================================================
// Helper: read just the header from a packet (non-destructive
// for the remaining data).
// Returns the PacketTypes value, or -1 on failure.
// ============================================================
inline Settings::PacketTypes ReadHeader(sf::Packet& p) {
	std::int32_t header;
	if (p >> header) {
		return static_cast<Settings::PacketTypes>(header);
	}
	return static_cast<Settings::PacketTypes>(-1);
}
