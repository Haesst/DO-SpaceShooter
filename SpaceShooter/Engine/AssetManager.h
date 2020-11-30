#pragma once

#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Engine;

class AssetManager
{
public:
	AssetManager();
	~AssetManager() = default;

	void Init(Engine* engine);
	void Clean();
	SDL_Texture* GetTexture(std::string id);
	void LoadTexture(std::string id, std::string path);
	TTF_Font* GetFont(std::string id);
	void LoadFont(std::string id, std::string path, int fontSize);

private:
	std::map<std::string, TTF_Font*> m_Fonts;
	std::map<std::string, SDL_Texture*> m_Textures;
	Engine* m_Engine;
};
