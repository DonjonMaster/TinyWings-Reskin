#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Component.h"
#include "Transform.h"

class Scene;

class GameObject
{
public:
	GameObject(sf::Vector2f pos, std::string name, Scene* scene);
	void Create();
	void Update(float dt);
	void Render(sf::RenderWindow* window);
	void Destroy();

	// Crée un component et l'ajoute dans le game object
	template<typename ComponentType, typename... Param>
	ComponentType* AddComponent(Param... parameters);

	// Récupère un component du game object
	// /!\ Peux être nullptr, à vérifier!
	template<typename ComponentType>
	ComponentType* GetComponent();

	// Récupère le transform du game object
	Transform& GetTransform();

	// Récupère le nom du game object
	std::string GetName();

	// Récupère la scène du game object
	Scene* GetScene();

	void SetZOrder(int z) { zOrder = z; }
	int GetZOrder() const { return zOrder; }

private:
	std::vector<Component*> components;
	Transform transform;
	std::string name;
	Scene* scene;

	int zOrder = 0;
};

#include "GameObject.inl"