#include "Engine.h"

#include <iostream>

#include "InputManager.h"

ECS::EntityManager* ECS::EntityManager::m_Instance = nullptr;

Engine::Engine()
{
}

Engine::~Engine()
{
	Clean();
}

void Engine::Quit()
{
	m_Running = false;
	Clean();
}

void Engine::Init()
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << SDL_GetError() << std::endl;
	}

	auto windowFlags = (SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_ALLOW_HIGHDPI);
	m_Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	if (!m_Window)
	{
		std::cerr << SDL_GetError() << std::endl;
		return;
	}

	m_Renderer = SDL_CreateRenderer(m_Window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

	if (!m_Renderer)
	{
		std::cerr << SDL_GetError() << std::endl;
		return;
	}

	m_EntityManager = &ECS::EntityManager::Get();
	m_InputManager.Init();

	m_Running = true;
}

void Engine::Clean()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Engine::Render(GameState currentState)
{
	if (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_MINIMIZED)
	{
		return;
	}

	SDL_SetRenderDrawColor(m_Renderer, m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
	SDL_RenderClear(m_Renderer);

	m_EntityManager->Render(currentState);

	SDL_RenderPresent(m_Renderer);
}

void Engine::Update(GameState currentState)
{
	m_EntityManager->Update(currentState);
}

void Engine::Event()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_Running = false;
			break;
		}
	}

	m_InputManager.Update();
}

SDL_Renderer* Engine::GetRenderer()
{
	return m_Renderer;
}

SDL_Window* Engine::GetWindow()
{
	return m_Window;
}

InputManager* Engine::GetInputManager()
{
	return &m_InputManager;
}

AssetManager* Engine::GetAssetManager()
{
	return &m_AssetManager;
}

