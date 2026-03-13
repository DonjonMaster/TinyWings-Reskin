#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "settings.h"
#include "SceneModule.h"


#include "Engine.h"
#include "Scene.h"

class World;

class Client
{
public :
	Client(World*& world);

	void ReceiveData();
	void SendStringMessage(const std::string& message);
	void SendData();
	void disconnect();
	// Ajouter des actions du joueur si nécessaire

	// Lancer le jeu (Player et monde)
	void run();

	void AttemptJoin();


	//void test()
	//{
	//	auto* engine = Engine::GetInstance();
	//	auto* sceneModule = engine->GetModuleManager()->GetModule<SceneModule>();

	//	Scene* currentScene = sceneModule->GetCurrentScene();

	//	GameObject* player = nullptr;
	//	for (auto* obj : currentScene->GetGameObjects()) {
	//		if (obj->GetName() == "GhostPlayer") {
	//			player = obj;
	//			break;
	//		}
	//	}

	//	auto* renderer = player->GetComponent<SpriteRendererComponent>();
	//	sf::Sprite* ghostSprite = renderer->GetSprite();
	//	ghostSprite->setColor(sf::Color(10, 10, 10, 10));
	//}

private:
	sf::IpAddress serverIp = sf::IpAddress::Any;
	unsigned short serverPort;

	sf::IpAddress adress;
	unsigned short port;
	sf::UdpSocket socket;

	World* world;
	// Ajouter les données à envoyer

	bool connected{ false };
};