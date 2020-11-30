#pragma once

#include <SDL_scancode.h>

#include "../Components/Player.h"
#include "../Components/Bullet.h"
#include "../../Engine/InputManager.h"
#include "../../ECS/Base/EntityManager.h"
#include "../../ECS/Components/Transform.h"
#include "../../ECS/Base/System.h"

struct PlayerSystem : ECS::System
{
	ECS::EntityManager* m_EntityManager;
	InputManager* m_InputManager = nullptr;

	PlayerSystem()
	{
		AddComponentSignature<Player>();
		m_EntityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{
		if (m_InputManager == nullptr)
		{
			m_InputManager = currentState.m_Engine->GetInputManager();
		}

		for (auto entity : m_Entities)
		{
			ECS::Transform* transform = &m_EntityManager->GetComponent<ECS::Transform>(entity);
			Player* player = &m_EntityManager->GetComponent<Player>(entity);

			MovePlayer(transform, player);

			if (player->m_CurrentTimeBetweenBullets <= 0.0f)
			{
				if (m_InputManager->IsKeyDown(SDL_SCANCODE_SPACE))
				{
					FireBullet(player, transform);
				}
			}
			else
			{
				player->m_CurrentTimeBetweenBullets -= currentState.m_DeltaTime;
			}
		}
	}

	virtual void MovePlayer(ECS::Transform* transform, Player* player)
	{
		if (m_InputManager->IsKeyDown(SDL_SCANCODE_D))
		{
			transform->Translate(Vector2D(1,0) * player->m_Speed);
		}

		if (m_InputManager->IsKeyDown(SDL_SCANCODE_A))
		{
			transform->Translate(Vector2D(-1, 0) * player->m_Speed);
		}

		if (m_InputManager->IsKeyDown(SDL_SCANCODE_W))
		{
			transform->Translate(Vector2D(0, -1) * player->m_Speed); // SDL flips y axis
		}

		if (m_InputManager->IsKeyDown(SDL_SCANCODE_S))
		{
			transform->Translate(Vector2D(0, 1) * player->m_Speed); // SDL flips y axis
		}
	}

	void FireBullet(Player* player, ECS::Transform* playerTransform)
	{
		ECS::EntityID bullet = player->m_BulletPool->Rent(false);
		Vector2D enemyPosition = playerTransform->m_Position;

		ECS::SpriteRenderer* spriteRenderer = &m_EntityManager->GetComponent<ECS::SpriteRenderer>(bullet);
		ECS::Transform* bulletTransform = &m_EntityManager->GetComponent<ECS::Transform>(bullet);

		bulletTransform->m_Position = enemyPosition + player->m_BulletOffset;
		m_EntityManager->GetComponent<Bullet>(bullet).m_Speed = player->m_BulletSpeed;

		UpdateBulletSpriteDestRect(spriteRenderer, bulletTransform);

		m_EntityManager->SetEntityActive(bullet, true);
		
		player->m_CurrentTimeBetweenBullets = player->m_TimeBetweenBullets;
	}

	void UpdateBulletSpriteDestRect(ECS::SpriteRenderer* spriteRenderer, ECS::Transform* bulletTransform)
	{
		spriteRenderer->m_DestRect.x = static_cast<int>(bulletTransform->m_Position.x);
		spriteRenderer->m_DestRect.y = static_cast<int>(bulletTransform->m_Position.y);
		spriteRenderer->m_DestRect.w = static_cast<int>(spriteRenderer->m_Width * bulletTransform->m_Scale.x);
		spriteRenderer->m_DestRect.h = static_cast<int>(spriteRenderer->m_Height * bulletTransform->m_Scale.y);
	}
};