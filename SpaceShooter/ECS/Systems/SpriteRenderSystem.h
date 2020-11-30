#pragma once

#include <iostream>

#include "../Base/System.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/Transform.h"

namespace ECS
{
	struct SpriteRenderSystem : public System
	{
		EntityManager* m_EntityManager;

		SpriteRenderSystem()
		{
			AddComponentSignature<Transform>();
			AddComponentSignature<SpriteRenderer>();

			m_EntityManager = &EntityManager::Get();
		}

		virtual void Update(GameState currentState) override
		{
			for (auto entity : m_Entities)
			{
				Transform* transform = &m_EntityManager->GetComponent<Transform>(entity);
				SpriteRenderer* spriteRenderer = &m_EntityManager->GetComponent<SpriteRenderer>(entity);

				spriteRenderer->m_DestRect.x = static_cast<int>(transform->m_Position.x);
				spriteRenderer->m_DestRect.y = static_cast<int>(transform->m_Position.y);
				spriteRenderer->m_DestRect.w = static_cast<int>(spriteRenderer->m_Width * transform->m_Scale.x);
				spriteRenderer->m_DestRect.h = static_cast<int>(spriteRenderer->m_Height * transform->m_Scale.y);
			}
		}

		virtual void Render(GameState currentState) override
		{
			for (auto entity : m_Entities)
			{
				if (EntityManager::Get().IsEntityActive(entity))
				{
					Transform* transform = &m_EntityManager->GetComponent<Transform>(entity);
					SpriteRenderer* spriteRenderer = &m_EntityManager->GetComponent<SpriteRenderer>(entity);
					SDL_RenderCopyEx(
						spriteRenderer->m_TargetRenderer,
						spriteRenderer->m_Texture,
						&spriteRenderer->m_SourceRect,
						&spriteRenderer->m_DestRect,
						transform->m_Rotation,
						nullptr,
						SDL_FLIP_NONE);
				}
			}
		}
	};
}