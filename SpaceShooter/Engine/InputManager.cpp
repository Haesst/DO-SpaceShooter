#include "InputManager.h"

#include <SDL_events.h>

void InputManager::Init()
{
	keys = const_cast<Uint8*>(SDL_GetKeyboardState(nullptr));
	lastKeys = keys;
}

void InputManager::Update()
{
	SDL_PumpEvents();

	lastKeys = keys;
	keys = const_cast<Uint8*>(SDL_GetKeyboardState(nullptr));
}

bool InputManager::IsKeyPressed(SDL_Scancode key) const
{
	return !lastKeys[key] && keys[key];
}

bool InputManager::IsKeyDown(SDL_Scancode key) const
{
	return keys[key];
}

bool InputManager::IsKeyReleased(SDL_Scancode key) const
{
	return lastKeys[key] && !keys[key];
}

