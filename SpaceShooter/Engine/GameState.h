#pragma once

class Engine;
struct SDL_Renderer;
struct SDL_Window;

struct GameState
{
	Engine* engine;
	float deltaTime = 0.1f;
	SDL_Renderer* renderer;
	SDL_Window* window;
};