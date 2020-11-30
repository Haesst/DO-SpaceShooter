#pragma once

#include <SDL_rect.h>

#include "../../ECS/Base/Component.h"

struct BoxCollider2D : public ECS::Component
{
	SDL_Rect m_Box{ 0,0,0,0 };
	
	float m_BaseWidth;
	float m_BaseHeight;

	BoxCollider2D(int baseWidth, int baseHeight)
		: m_BaseWidth(baseWidth), m_BaseHeight(baseHeight)
	{}
};