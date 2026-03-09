#pragma once
#include "Module.h"
#include <SFML/Graphics.hpp>
#include <string>

class ResourceModule : public Module
{
public:
	sf::Texture& GetTexture(std::string path);
	sf::Font& GetFont(std::string path);

private:
	std::unordered_map<std::string, sf::Texture*> textureMap;
	std::unordered_map<std::string, sf::Font*> fontMap;
};