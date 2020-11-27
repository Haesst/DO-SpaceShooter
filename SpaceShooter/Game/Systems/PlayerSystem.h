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
	ECS::EntityManager* entityManager;
	InputManager* inputManager = nullptr;

	PlayerSystem()
	{
		AddComponentSignature<Player>();
		entityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{
		if (inputManager == nullptr)
		{
			inputManager = currentState.engine->GetInputManager();
		}

		for (auto entity : entities)
		{
			ECS::Transform* transform = &entityManager->GetComponent<ECS::Transform>(entity);
			Player* player = &entityManager->GetComponent<Player>(entity);

			MovePlayer(transform, player);

			if (player->currentTimeBetweenBullets <= 0.0f)
			{
				if (inputManager->IsKeyDown(SDL_SCANCODE_SPACE))
				{
					FireBullet(player, transform);
				}
			}
			else
			{
				player->currentTimeBetweenBullets -= currentState.deltaTime;
			}
		}
	}

	virtual void MovePlayer(ECS::Transform* transform, Player* player)
	{
		if (inputManager->IsKeyDown(SDL_SCANCODE_D))
		{
			transform->Translate(Vector2D(1,0) * player->speed);
		}

		if (inputManager->IsKeyDown(SDL_SCANCODE_A))
		{
			transform->Translate(Vector2D(-1, 0) * player->speed);
		}

		if (inputManager->IsKeyDown(SDL_SCANCODE_W))
		{
			transform->Translate(Vector2D(0, -1) * player->speed); // SDL flips y axis
		}

		if (inputManager->IsKeyDown(SDL_SCANCODE_S))
		{
			transform->Translate(Vector2D(0, 1) * player->speed); // SDL flips y axis
		}
	}

	void FireBullet(Player* player, ECS::Transform* playerTransform)
	{
		ECS::EntityID bullet = player->bulletPool->Rent(false);
		Vector2D enemyPosition = playerTransform->Position;

		ECS::SpriteRenderer* spriteRenderer = &entityManager->GetComponent<ECS::SpriteRenderer>(bullet);
		ECS::Transform* bulletTransform = &entityManager->GetComponent<ECS::Transform>(bullet);

		bulletTransform->Position = enemyPosition + player->bulletOffset;
		entityManager->GetComponent<Bullet>(bullet).speed = player->bulletSpeed;

		UpdateBulletSpriteDestRect(spriteRenderer, bulletTransform);

		entityManager->SetEntityActive(bullet, true);
		
		player->currentTimeBetweenBullets = player->timeBetweenBullets;
	}

	void UpdateBulletSpriteDestRect(ECS::SpriteRenderer* spriteRenderer, ECS::Transform* bulletTransform)
	{
		spriteRenderer->destRect.x = static_cast<int>(bulletTransform->Position.x);
		spriteRenderer->destRect.y = static_cast<int>(bulletTransform->Position.y);
		spriteRenderer->destRect.w = static_cast<int>(spriteRenderer->width * bulletTransform->Scale.x);
		spriteRenderer->destRect.h = static_cast<int>(spriteRenderer->height * bulletTransform->Scale.y);
	}
};