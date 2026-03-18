#include "Client.h"
#include "World.h"

Client::Client(World *& world) : adress(sf::IpAddress::Any) {
	this->world = world;
	// Ajouter en paramètre les données à transférer
	auto localIp = sf::IpAddress::getLocalAddress();
	if (localIp) {
		adress = *localIp;
	}
}

bool Client::ReceiveData() {
	sf::Packet p;
	std::optional<sf::IpAddress> sIp;
	unsigned short sP;
	p.clear();
	
	sf::Socket::Status test = socket.receive(p, sIp, sP);
	
	if (test == sf::Socket::Status::NotReady) {
		std::cout << "No packets to be collected" << std::endl;
		return false;
	}

	if (test == sf::Socket::Status::Done) {
		int header;
		std::cout << "Tried to extract header " << std::endl;
		if (!(p >> header)) return true;
		std::cout << "Header extraction succesfull" << std::endl << std::endl;

		switch (Settings::PacketTypes(header)) {
		case Settings::PacketTypes::NEW_CONNECTION: {
			std::string k;
			int num;
			if (p >> k >> num) {
				std::cout << "Connecté en tant que joueur " << num << " (ID: " << k << ")\n";
				// Une fois connecté, on envoie un message de test
				SendStringMessage("Bonjour serveur, je suis connecte !");
			}

			break;
		}
		case Settings::PacketTypes::START_GAME: {
			std::cout << "Le serveur à lancé la partie" << std::endl;
			world->StartGame();
			world->state = GameState::PLAYING;
			break;
		}
		case Settings::PacketTypes::PLAYER_DATA: {
			std::cout << std::endl << "Received Player Data" << std::endl;
			std::string id;
			float x, y;
			if (p >> id >> x >> y) {
				if (world->currentScene) {
					world->currentScene->UpdatePos({ x, y });
				}

				if (world->remotePlayers.count(id)) {
					world->remotePlayers[id]->GetTransform().pos = { x, y };
				}
			}
			break;
		}
		case Settings::PacketTypes::DISCONNECT: {
			std::string k;
			p >> k;
			std::cout << "Player: " << k << " disconnected.\n";
			break;
		}
		default:
			std::cout << "Error while receiving data : no PacketTypes : " << Settings::PacketTypes(header) << std::endl;
			break;
		}
	}
	if (test == sf::Socket::Status::Error) {
		std::cout << "Error with socket status" << std::endl;
	}


	p.clear();

	return true;
}

void Client::SendStringMessage(const std::string& message) {
	sf::Packet p;
	p << static_cast<int>(Settings::PacketTypes::STRING_MESSAGE) << message;

	if (socket.send(p, serverIp, serverPort) != sf::Socket::Status::Done) {
		std::cout << "Erreur lors de l'envoi du message texte.\n";
	}
}

void Client::SendData() {
	if (!world || !world->playerContext.player) {
		return;
	}

	sf::Packet p;
	p << static_cast<int>(Settings::PacketTypes::PLAYER_DATA);

	auto& transform = world->playerContext.player->GetTransform();
	p << transform.pos.x << transform.pos.y;
	// Ajouter l'envoi de la position
	if (socket.send(p, serverIp, serverPort) == sf::Socket::Status::Done) {

	}
}

void Client::disconnect() {
	sf::Packet p;
	p << Settings::PacketTypes::DISCONNECT << port;

	std::cout << port << std::endl;

	if (socket.send(p, serverIp, serverPort) == sf::Socket::Status::Done) {

	}

	socket.unbind();
	connected = false;
}

void Client::run(){
	// Tentative de connexion
	if (world->uiw.attemptJoin) {
		AttemptJoin();
		world->uiw.attemptJoin = false;
	}

	// Réception des données
	if (world->state == GameState::WATINGFORHOST || world->state == GameState::PLAYING) {
		while (ReceiveData());
	}

	// Envoi des données si on joue
	if (world->state == GameState::PLAYING) {
		SendData();
	}
}

void Client::AttemptJoin() {

	socket.setBlocking(false);
	// Récupérer ce que le joueur entre comme input lors d'une connexion (dans les menus)
	auto resolvedIp = sf::IpAddress::resolve(world->serverIPInput);
	if (!resolvedIp) {
		std::cout << "IP invalide\n";
		return;
	}

	serverIp = *resolvedIp;

	serverPort = static_cast<unsigned short>(std::stoul(world->serverPortInput));
	port = static_cast<unsigned short>(std::stoul(world->userPortInput));

	if (socket.bind(port) != sf::Socket::Status::Done) {
		std::cout << "Impossible de lier le socket au port donné : " << port << std::endl;
		return;
	}

	sf::Packet p;
	p << Settings::PacketTypes::NEW_CONNECTION << port;

	if (socket.send(p, serverIp, serverPort) != sf::Socket::Status::Done) {
		std::cout << "Impossible de se connecter au server" << std::endl;
	}
	else {
		std::cout << "Connexion avec le serveur établie" << std::endl;
		world->state = GameState::WATINGFORHOST;
	}

	p.clear();

	ReceiveData();

	// Ajouter les changements liée au player

	socket.setBlocking(false);
	p.clear();

	connected = true;

	// Attendre le lancement d'une partie
	world->uiw.attemptJoin = false;
}
