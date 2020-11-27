#include "AssetManager.h"

#include <iostream>

#include "Engine.h"

AssetManager::AssetManager()
{
	if (TTF_Init() != 0)
	{
		std::cerr << TTF_GetError() << std::endl;
	}

	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
	{
		std::cerr << IMG_GetError() << std::endl;
	}
}

void AssetManager::Init(Engine* engine)
{
	this->engine = engine;
}

void AssetManager::Clean()
{
	auto textureIt = textures.begin();
	while (textureIt != textures.end())
	{
		SDL_DestroyTexture(textureIt->second);
		textureIt = textures.erase(textureIt);
	}

	textures.clear();

	auto fontIt = fonts.begin();
	while (fontIt != fonts.end())
	{
		TTF_CloseFont(fontIt->second);
		fontIt = fonts.erase(fontIt);
	}

	fonts.clear();

	std::cout << "Assets cleared" << std::endl;
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return (textures.count(id) > 0) ? textures[id] : nullptr;
}

void AssetManager::LoadTexture(std::string id, std::string path)
{
	if (textures.count(id) <= 0)
	{
		SDL_Texture* texture = IMG_LoadTexture(engine->GetRenderer(), path.c_str());

		if (texture != nullptr)
		{
			textures[id] = texture;
			std::cout << "Texture: [" << path << "] loaded!" << std::endl;
		}
		else
		{
			std::cerr << SDL_GetError() << std::endl;
		}
	}
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	return (fonts.count(id) > 0) ? fonts[id] : nullptr;
}

void AssetManager::LoadFont(std::string id, std::string path, int fontSize)
{
	TTF_Font* newFont = TTF_OpenFont(path.c_str(), fontSize);

	if (newFont != nullptr)
	{
		fonts.emplace(id, newFont);
		std::cout << "Font: [" << path << "] loaded!" << std::endl;
	}
	else
	{
		std::cout << TTF_GetError() << std::endl;
	}
}