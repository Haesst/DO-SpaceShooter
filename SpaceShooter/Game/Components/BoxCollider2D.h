#pragma once

#include <SDL_rect.h>

#include "../../ECS/Base/Component.h"

struct BoxCollider2D : public ECS::Component
{
	SDL_Rect Box{ 0,0,0,0 };
	
	float baseWidth;
	float baseHeight;

	BoxCollider2D(int baseWidth, int baseHeight)
		: baseWidth(baseWidth), baseHeight(baseHeight)
	{}
};