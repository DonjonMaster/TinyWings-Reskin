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

void Client::ReceiveData() {
	sf::Packet p;
	std::optional<sf::IpAddress> sIp;
	unsigned short sP;

	p.clear();

	if (socket.receive(p, sIp, sP) == sf::Socket::Status::Done) {
		int header;
		if (!(p >> header)) return;

		switch (Settings::PacketTypes(header)) {
		case Settings::PacketTypes::NEW_CONNECTION: {
			std::string k;
			int num;
			if (p >> num >> k) {
				std::cout << "Connecté en tant que joueur " << num << " (ID: " << k << ")\n";
				// Une fois connecté, on envoie un message de test
				SendStringMessage("Bonjour serveur, je suis connecte !");
			}
			// Ajouter le joueur visuellement

			break;
		}
		case Settings::PacketTypes::DISCONNECT: {
			std::string k;
			p >> k;
			std::cout << "Player: " << k << " disconnected.\n";
			break;
		}
		default:
			break;
		}
	}

	p.clear();
}

void Client::SendStringMessage(const std::string& message) {
	sf::Packet p;
	p << static_cast<int>(Settings::PacketTypes::STRING_MESSAGE) << message;

	if (socket.send(p, serverIp, serverPort) != sf::Socket::Status::Done) {
		std::cout << "Erreur lors de l'envoi du message texte.\n";
	}
}

void Client::SendData() {
	std::string msg;
	sf::Packet p;

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
	sf::Clock clock;
	sf::Time t{ sf::Time::Zero };
	sf::Time dt{ sf::seconds(1.0f / 60.f) };
	
	while (world->window.isOpen()) {
		world->processEvents();

		if (world->state == GameState::PLAYING) {
			ReceiveData();
		}

		// On essaye de rejoindre un serveur
		if (world->uiw.attemptJoin) {
			AttemptJoin();
		}

		t += clock.restart();
		while (t > dt) {
			t -= dt;

			// On échanges les données en jeu
			if (world->state == GameState::PLAYING) {
				SendData();
				ReceiveData();
			}

			// Update du jeu
			world->update(dt.asSeconds());
		}

		if (world->state == GameState::HOSTING) {
			world->update(dt.asSeconds());
		}
		
		world->render();
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
	}

	p.clear();

	ReceiveData();

	// Ajouter les changements liée au player

	socket.setBlocking(false);
	p.clear();

	connected = true;

	// Attendre le lancement d'une partie
	world->state = GameState::PLAYING;
	world->uiw.attemptJoin = false;
}