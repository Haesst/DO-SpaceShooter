#pragma once

#include <SDL_scancode.h>

class InputManager
{
private:
	Uint8* keys = nullptr;
	Uint8* lastKeys = nullptr;

public:
	void Init();
	void Update();
	bool IsKeyPressed(SDL_Scancode key) const;
	bool IsKeyDown(SDL_Scancode key) const;
	bool IsKeyReleased(SDL_Scancode key) const;
};
