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
	this->m_Engine = engine;
}

void AssetManager::Clean()
{
	auto textureIt = m_Textures.begin();
	while (textureIt != m_Textures.end())
	{
		SDL_DestroyTexture(textureIt->second);
		textureIt = m_Textures.erase(textureIt);
	}

	m_Textures.clear();

	auto fontIt = m_Fonts.begin();
	while (fontIt != m_Fonts.end())
	{
		TTF_CloseFont(fontIt->second);
		fontIt = m_Fonts.erase(fontIt);
	}

	m_Fonts.clear();

	std::cout << "Assets cleared" << std::endl;
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return (m_Textures.count(id) > 0) ? m_Textures[id] : nullptr;
}

void AssetManager::LoadTexture(std::string id, std::string path)
{
	if (m_Textures.count(id) <= 0)
	{
		SDL_Texture* texture = IMG_LoadTexture(m_Engine->GetRenderer(), path.c_str());

		if (texture != nullptr)
		{
			m_Textures[id] = texture;
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
	return (m_Fonts.count(id) > 0) ? m_Fonts[id] : nullptr;
}

void AssetManager::LoadFont(std::string id, std::string path, int fontSize)
{
	TTF_Font* newFont = TTF_OpenFont(path.c_str(), fontSize);

	if (newFont != nullptr)
	{
		m_Fonts.emplace(id, newFont);
		std::cout << "Font: [" << path << "] loaded!" << std::endl;
	}
	else
	{
		std::cout << TTF_GetError() << std::endl;
	}
}