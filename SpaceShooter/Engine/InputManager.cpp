#include "InputManager.h"

#include <SDL_events.h>

void InputManager::Init()
{
	m_Keys = const_cast<Uint8*>(SDL_GetKeyboardState(nullptr));
	m_LastKeys = m_Keys;
}

void InputManager::Update()
{
	SDL_PumpEvents();

	m_LastKeys = m_Keys;
	m_Keys = const_cast<Uint8*>(SDL_GetKeyboardState(nullptr));
}

bool InputManager::IsKeyPressed(SDL_Scancode key) const
{
	return !m_LastKeys[key] && m_Keys[key];
}

bool InputManager::IsKeyDown(SDL_Scancode key) const
{
	return m_Keys[key];
}

bool InputManager::IsKeyReleased(SDL_Scancode key) const
{
	return m_LastKeys[key] && !m_Keys[key];
}

