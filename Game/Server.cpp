#include "Server.h"

Server::Server() : serverIp(sf::IpAddress::Any) {
	// On évite de freeze le jeu lors de l'attente de paquets
	serverSocket.setBlocking(false);
	auto localIp = sf::IpAddress::getLocalAddress();
	if (localIp) {
		serverIp = *localIp;
	}
}

void Server::Run() {
	ReceiveData();
}

bool Server::Init() {
	if (serverSocket.bind(serverPort) != sf::Socket::Status::Done) {
		std::cout << "Impossible de lier le socket serveur au port donné : " << serverPort << std::endl;
		return false;
	}
	// On évite de freeze le jeu lors de l'attente de paquets
	serverSocket.setBlocking(false);
	return true;
}

void Server::ReceiveData() {
	sf::Packet packet;
	std::optional<sf::IpAddress> senderIp;
	unsigned short senderPort;
	int header;

	packet.clear();

	if (serverSocket.receive(packet, senderIp, senderPort) == sf::Socket::Status::Done) {

		if (!(packet >> header) || !senderIp) return;

		// On utilise l'Ip extraite
		std::string senderKey = senderIp->toString() + ":" + std::to_string(senderPort);

		switch (Settings::PacketTypes(header)) {
		case Settings::PacketTypes::NEW_CONNECTION: {
			if (connections.size() == 2) {
				std::cout << "Un joueur a essayé de se connecter. Le lobby est plein." << std::endl;
				return;
			}

			Connection c;
			c.address = *senderIp;
			c.port = senderPort;
			c.playerNumber = connections.size() + 1;

			connections[senderKey] = c;

			std::cout << "Joueur " << c.playerNumber << " connecte : " << senderKey << std::endl;

			sf::Packet reply;
			reply << Settings::PacketTypes::NEW_CONNECTION << c.playerNumber;

			if (serverSocket.send(reply, *senderIp, senderPort) != sf::Socket::Status::Done)
			{
				std::cout << "Erreur lors de l'envoi du packet au client" << std::endl;
			}

			SendInitialData(c);
			std::cout << "Les données initales du Joueur 1 ont été envoyées" << std::endl;

			std::cout << "Nouvelle connexion avec " << *senderIp << c.port << std::endl;
			std::cout << "Connexions total : " << connections.size() << std::endl;

			break;
		}
		case Settings::STRING_MESSAGE: {
			std::string receivedMsg;
			if (packet >> receivedMsg) {
				std::cout << "[MESSAGE de " << senderKey << "] : " << receivedMsg << std::endl;
			}
			break;
		}
		case Settings::PacketTypes::DISCONNECT: {
			std::cout << "Deconnexion de : " << senderKey << std::endl;

			sf::Packet toSendPacket;
			toSendPacket << static_cast<int>(Settings::PacketTypes::DISCONNECT) << senderKey;

			SendData(senderKey, toSendPacket);
			connections.erase(senderKey);
			std::cout << "Connexions total : " << connections.size() << std::endl;
			break;
		}
		default:
			break;

		}
	}
	packet.clear();
}

void Server::SendData(std::string& sender, sf::Packet& p)
{
	for (const auto& [key, val] : connections) {
		if (key != sender) {
			if (serverSocket.send(p, val.address, val.port) == sf::Socket::Status::Done) {

			}
		}
	}
}

void Server::SendInitialData(Connection& c)
{
	sf::Packet p;

	for (const auto& [key, val] : connections) {
		p << Settings::PacketTypes::NEW_CONNECTION << key << val.playerNumber;
		if (serverSocket.send(p, c.address, c.port) == sf::Socket::Status::Done) {

		}
		p.clear();
	}
}

void Server::SendDataToEveryone(sf::Packet& p)
{
	for (const auto& [key, val] : connections) {
		if (serverSocket.send(p, val.address, val.port) == sf::Socket::Status::Done) {

		}
	}
}

sf::IpAddress Server::GetIp()
{
	return serverIp;
}
