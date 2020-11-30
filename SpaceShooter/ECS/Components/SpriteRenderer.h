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
		int m_Width;
		int m_Height;
		SDL_Rect m_SourceRect = { 0,0,0,0 };
		SDL_Rect m_DestRect = { 0,0,0,0 };

		std::string m_TextureID = "";
		SDL_Texture* m_Texture;
		SDL_Renderer* m_TargetRenderer;

		SpriteRenderer(SDL_Renderer* target, SDL_Texture* texture)
			: m_TargetRenderer(target), m_Texture(texture)
		{
			SDL_QueryTexture(texture, nullptr, nullptr, &m_Width, &m_Height);

			Transform transform = EntityManager::Get().GetComponent<Transform>(m_EntityID);

			m_DestRect.x = transform.m_Position.x;
			m_DestRect.y = transform.m_Position.y;
			m_DestRect.w = m_Width * transform.m_Scale.x;
			m_DestRect.h = m_Height * transform.m_Scale.y;

			m_SourceRect.x = 0;
			m_SourceRect.y = 0;
			m_SourceRect.w = m_Width;
			m_SourceRect.h = m_Height;
		}

		~SpriteRenderer() = default;
	};
}