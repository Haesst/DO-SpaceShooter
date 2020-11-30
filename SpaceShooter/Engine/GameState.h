#pragma once

class Engine;
struct SDL_Renderer;
struct SDL_Window;

struct GameState
{
	Engine* m_Engine;
	float m_DeltaTime = 0.1f;
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
};