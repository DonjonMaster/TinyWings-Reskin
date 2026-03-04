#include "GameObject.h"
#include <iostream>

GameObject::GameObject(sf::Vector2f pos, std::string name, Scene* scene) :
	name(name),
	scene(scene)
{

	transform.pos = pos;

};

void GameObject::Create()
{

	for (Component* component : components)
		component->Create();

}

void GameObject::Update(float dt)
{

	for (Component* component : components)
		component->Update(dt);

}

void GameObject::Render(sf::RenderWindow* window)
{

	for (Component* component : components)
	{
		component->Render(window);
	}

}

void GameObject::Destroy()
{

	for (Component* component : components) {
		component->Destroy();
		delete component;
	}

}

Transform& GameObject::GetTransform()
{

	return transform;

}

std::string GameObject::GetName()
{

	return name;

}

Scene* GameObject::GetScene()
{

	return scene;

}