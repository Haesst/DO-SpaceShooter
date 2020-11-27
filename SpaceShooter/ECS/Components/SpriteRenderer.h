#pragma once

#include <string>
#include <SDL.h>

#include "../Base/Component.h"
#include "../../Engine/AssetManager.h"
#include "../Base/EntityManager.h"
#include "Transform.h"

namespace ECS
{
	struct SpriteRenderer : public Component
	{
		int width;
		int height;
		SDL_Rect sourceRect = { 0,0,0,0 };
		SDL_Rect destRect = { 0,0,0,0 };

		std::string textureID = "";
		SDL_Texture* texture;
		SDL_Renderer* targetRenderer;

		SpriteRenderer(SDL_Renderer* target, SDL_Texture* texture)
			: targetRenderer(target), texture(texture)
		{
			SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

			Transform transform = EntityManager::Get().GetComponent<Transform>(entityID);

			destRect.x = transform.Position.x;
			destRect.y = transform.Position.y;
			destRect.w = width * transform.Scale.x;
			destRect.h = height * transform.Scale.y;

			sourceRect.x = 0;
			sourceRect.y = 0;
			sourceRect.w = width;
			sourceRect.h = height;
		}

		~SpriteRenderer() = default;
	};
}