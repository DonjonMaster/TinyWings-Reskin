#pragma once
#include <iostream>
#include "GameObject.h"

class Scene
{
public:
	virtual void Create();
	virtual void Update(float dt);
	virtual void Render(sf::RenderWindow* window);
	virtual void Destroy();
	virtual void UpdatePos(sf::Vector2f pos);

	// Créer un game object dans la scène
	GameObject* CreateGameObject(sf::Vector2f pos, std::string name);

	// Ajoute en fil d'attente de suppresion du game object
	void DeleteGameObject(GameObject* gameObject);

	// Récupère les game objects dans la scène ayant le nom correspondant
	std::vector<GameObject*> GetGameObjectsWithName(std::string name);

	const std::vector<GameObject*>& GetGameObjects() const { return gameObjects; }

	void SetName(std::string _name) { name = _name; }
	std::string GetName() const { return name; }

protected:
	std::string name;

private:
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> gameObjectsToDelete;

	GameObject* ghost;
};