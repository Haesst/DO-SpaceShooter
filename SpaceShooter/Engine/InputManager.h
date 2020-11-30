#pragma once

#include <SDL_scancode.h>

class InputManager
{
private:
	Uint8* m_Keys = nullptr;
	Uint8* m_LastKeys = nullptr;

public:
	void Init();
	void Update();
	bool IsKeyPressed(SDL_Scancode key) const;
	bool IsKeyDown(SDL_Scancode key) const;
	bool IsKeyReleased(SDL_Scancode key) const;
};
