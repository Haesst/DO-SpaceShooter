#include "Engine.h"

#include <iostream>

#include "InputManager.h"

ECS::EntityManager* ECS::EntityManager::instance = nullptr;

Engine::Engine()
{
}

Engine::~Engine()
{
	Clean();
}

void Engine::Quit()
{
	Running = false;
	Clean();
}

void Engine::Init()
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << SDL_GetError() << std::endl;
	}

	auto windowFlags = (SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_ALLOW_HIGHDPI);
	Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	if (!Window)
	{
		std::cerr << SDL_GetError() << std::endl;
		return;
	}

	Renderer = SDL_CreateRenderer(Window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

	if (!Renderer)
	{
		std::cerr << SDL_GetError() << std::endl;
		return;
	}

	entityManager = &ECS::EntityManager::Get();
	inputManager.Init();

	Running = true;
}

void Engine::Clean()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Engine::Render(GameState currentState)
{
	if (SDL_GetWindowFlags(Window) & SDL_WINDOW_MINIMIZED)
	{
		return;
	}

	SDL_SetRenderDrawColor(Renderer, ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
	SDL_RenderClear(Renderer);

	entityManager->Render(currentState);

	SDL_RenderPresent(Renderer);
}

void Engine::Update(GameState currentState)
{
	entityManager->Update(currentState);
}

void Engine::Event()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Running = false;
			break;
		}
	}

	inputManager.Update();
}

SDL_Renderer* Engine::GetRenderer()
{
	return Renderer;
}

SDL_Window* Engine::GetWindow()
{
	return Window;
}

InputManager* Engine::GetInputManager()
{
	return &inputManager;
}

AssetManager* Engine::GetAssetManager()
{
	return &assetManager;
}

