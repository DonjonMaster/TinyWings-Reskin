#include "ResourceModule.h"
#include <iostream>

sf::Texture& ResourceModule::GetTexture(std::string path)
{

	if (!textureMap.contains(path))
		textureMap[path] = new sf::Texture(path);

	return *textureMap[path];

}

sf::Font& ResourceModule::GetFont(std::string path)
{

	if (!fontMap.contains(path))
		fontMap[path] = new sf::Font(path);

	return *fontMap[path];

}