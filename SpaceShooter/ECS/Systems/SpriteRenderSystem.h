#pragma once

#include <iostream>

#include "../Base/System.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/Transform.h"

namespace ECS
{
	struct SpriteRenderSystem : public System
	{
		EntityManager* entityManager;

		SpriteRenderSystem()
		{
			AddComponentSignature<Transform>();
			AddComponentSignature<SpriteRenderer>();

			entityManager = &EntityManager::Get();
		}

		virtual void Update(GameState currentState) override
		{
			for (auto entity : entities)
			{
				Transform* transform = &entityManager->GetComponent<Transform>(entity);
				SpriteRenderer* spriteRenderer = &entityManager->GetComponent<SpriteRenderer>(entity);

				spriteRenderer->destRect.x = static_cast<int>(transform->Position.x);
				spriteRenderer->destRect.y = static_cast<int>(transform->Position.y);
				spriteRenderer->destRect.w = static_cast<int>(spriteRenderer->width * transform->Scale.x);
				spriteRenderer->destRect.h = static_cast<int>(spriteRenderer->height * transform->Scale.y);
			}
		}

		virtual void Render(GameState currentState) override
		{
			for (auto entity : entities)
			{
				if (EntityManager::Get().IsEntityActive(entity))
				{
					Transform* transform = &entityManager->GetComponent<Transform>(entity);
					SpriteRenderer* spriteRenderer = &entityManager->GetComponent<SpriteRenderer>(entity);
					SDL_RenderCopyEx(
						spriteRenderer->targetRenderer,
						spriteRenderer->texture,
						&spriteRenderer->sourceRect,
						&spriteRenderer->destRect,
						transform->Rotation,
						nullptr,
						SDL_FLIP_NONE);
				}
			}
		}
	};
}