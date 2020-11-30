#pragma once

#include <SDL.h>

#include "GameState.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "../ECS/Base/EntityManager.h"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 1280;
constexpr SDL_Color DARK = { 30, 30, 30, 255 };


class Engine
{
private:
	bool m_Running;
	SDL_Window* m_Window;
	SDL_Color m_ClearColor;
	SDL_Renderer* m_Renderer;
	ECS::EntityManager* m_EntityManager;
	InputManager m_InputManager;
	AssetManager m_AssetManager;

public:
	Engine();
	~Engine();

	void Quit();
	void Init();
	void Clean();

	void Render(GameState currentState);
	void Update(GameState currentState);
	void Event();

	SDL_Renderer* GetRenderer();
	SDL_Window* GetWindow();
	InputManager* GetInputManager();
	AssetManager* GetAssetManager();

	inline bool IsRunning()
	{
		return m_Running;
	}
};