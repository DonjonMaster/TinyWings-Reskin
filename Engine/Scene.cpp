#include "Scene.h"

void Scene::Create()
{

	for (GameObject* gameObject : gameObjects)
		gameObject->Create();

}

void Scene::Update(float dt)
{

	std::vector<GameObject*> preGameObjects = gameObjects;

	for (GameObject* gameObject : preGameObjects)
		gameObject->Update(dt);

	if (gameObjectsToDelete.size() > 0) {

		for (GameObject* gameObject : gameObjectsToDelete) {

			auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
			if (it != gameObjects.end()) {
				std::iter_swap(it, gameObjects.end() - 1);
				gameObjects.pop_back();
			}

			gameObject->Destroy();
			delete gameObject;

		}

		gameObjectsToDelete.clear();

	}

}

void Scene::Render(sf::RenderWindow* window)
{
	// Dans la fonction qui dessine la scène
	std::sort(gameObjects.begin(), gameObjects.end(), [](GameObject* a, GameObject* b) {
		return a->GetZOrder() < b->GetZOrder();
		});

	for (auto* obj : gameObjects) {
		obj->Render(window);
	}

}

void Scene::Destroy()
{

	for (GameObject* gameObject : gameObjects)
		gameObject->Destroy();

}

GameObject* Scene::CreateGameObject(sf::Vector2f pos, std::string name)
{

	GameObject* gameObject = new GameObject(pos, name, this);
	gameObjects.push_back(gameObject);
	return gameObject;

}

void Scene::DeleteGameObject(GameObject* gameObject)
{

	gameObjectsToDelete.push_back(gameObject);

}

std::vector<GameObject*> Scene::GetGameObjectsWithName(std::string name)
{

	std::vector<GameObject*> gameObjectsWithName;

	for (GameObject* gameObject : gameObjects)
		if (gameObject->GetName() == name)
			gameObjectsWithName.push_back(gameObject);

	return gameObjectsWithName;

}