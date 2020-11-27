#pragma once

#include "../../ECS/Base/EntityManager.h"
#include "../../ECS/Base/System.h"
#include "../../ECS/Components/SpriteRenderer.h"
#include "../../ECS/Components/Transform.h"
#include "../Components/Bullet.h"
#include "../Components/SmallEnemy.h"

struct SmallEnemySystem : ECS::System
{
	ECS::EntityManager* entityManager;

	SmallEnemySystem()
	{
		AddComponentSignature<SmallEnemy>();
		entityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{	
		int minimumX = 0;
		int currentWindowWidth;
		int currentWindowHeight;

		SDL_GetWindowSize(currentState.window, &currentWindowWidth, &currentWindowHeight);

		for (auto entity : entities)
		{
			ECS::Transform* transform = &entityManager->GetComponent<ECS::Transform>(entity);
			ECS::SpriteRenderer* spriteRenderer = &entityManager->GetComponent<ECS::SpriteRenderer>(entity);
			SmallEnemy* enemy = &entityManager->GetComponent<SmallEnemy>(entity);

			int maximumWidth = currentWindowWidth - spriteRenderer->width;

			MoveEnemy(enemy, transform, minimumX, maximumWidth);

			enemy->timeSinceLastShot -= currentState.deltaTime;

			if (enemy->timeSinceLastShot <= 0.0f)
			{
				FireBullet(enemy, currentState.deltaTime);
				enemy->timeSinceLastShot = enemy->shotTimer;
			}
		}
	}

	void MoveEnemy(SmallEnemy* enemy, ECS::Transform* enemyTransform, int minimumX, int maximumX)
	{
		enemyTransform->Translate(enemy->currentDirection * enemy->speed);

		if (enemy->currentDirection.x < 0)
		{
			if (enemyTransform->Position.x < minimumX)
			{
				enemy->currentDirection = { 1, enemy->currentDirection.y };
			}
		}
		else
		{
			if (enemyTransform->Position.x > maximumX)
			{
				enemy->currentDirection = { -1, enemy->currentDirection.y };
			}
		}
	}

	void FireBullet(SmallEnemy* enemy, float deltaTime)
	{

		enemy->timeSinceLastShot -= deltaTime;

		if (enemy->timeSinceLastShot <= 0.0f)
		{
			ECS::EntityID bullet = enemy->bulletPool->Rent(false);
			Vector2D enemyPosition = entityManager->GetComponent<ECS::Transform>(enemy->GetID()).Position;

			ECS::SpriteRenderer* spriteRenderer = &entityManager->GetComponent<ECS::SpriteRenderer>(bullet);
			ECS::Transform* bulletTransform = &entityManager->GetComponent<ECS::Transform>(bullet);

			bulletTransform->Position = enemyPosition + enemy->bulletOffset;
			entityManager->GetComponent<Bullet>(bullet).speed = enemy->bulletSpeed;
			
			UpdateBulletSpriteDestRect(spriteRenderer, bulletTransform);
			
			entityManager->SetEntityActive(bullet, true);

			enemy->timeSinceLastShot = enemy->shotTimer;
		}
	}

	void UpdateBulletSpriteDestRect(ECS::SpriteRenderer* spriteRenderer, ECS::Transform* bulletTransform)
	{
		spriteRenderer->destRect.x = static_cast<int>(bulletTransform->Position.x);
		spriteRenderer->destRect.y = static_cast<int>(bulletTransform->Position.y);
		spriteRenderer->destRect.w = static_cast<int>(spriteRenderer->width * bulletTransform->Scale.x);
		spriteRenderer->destRect.h = static_cast<int>(spriteRenderer->height * bulletTransform->Scale.y);
	}
};