#pragma once

#include "../../ECS/Base/EntityManager.h"
#include "../../ECS/Base/System.h"
#include "../../ECS/Components/SpriteRenderer.h"
#include "../../ECS/Components/Transform.h"
#include "../Components/Bullet.h"
#include "../Components/SmallEnemy.h"

struct SmallEnemySystem : ECS::System
{
	ECS::EntityManager* m_EntityManager;

	SmallEnemySystem()
	{
		AddComponentSignature<SmallEnemy>();
		m_EntityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{	
		int minimumX = 0;
		int currentWindowWidth;
		int currentWindowHeight;

		SDL_GetWindowSize(currentState.m_Window, &currentWindowWidth, &currentWindowHeight);

		for (auto entity : m_Entities)
		{
			ECS::Transform* transform = &m_EntityManager->GetComponent<ECS::Transform>(entity);
			ECS::SpriteRenderer* spriteRenderer = &m_EntityManager->GetComponent<ECS::SpriteRenderer>(entity);
			SmallEnemy* enemy = &m_EntityManager->GetComponent<SmallEnemy>(entity);

			int maximumWidth = currentWindowWidth - spriteRenderer->m_Width;

			MoveEnemy(enemy, transform, minimumX, maximumWidth);

			enemy->m_TimeSinceLastShot -= currentState.m_DeltaTime;

			if (enemy->m_TimeSinceLastShot <= 0.0f)
			{
				FireBullet(enemy, currentState.m_DeltaTime);
				enemy->m_TimeSinceLastShot = enemy->m_ShotTimer;
			}
		}
	}

	void MoveEnemy(SmallEnemy* enemy, ECS::Transform* enemyTransform, int minimumX, int maximumX)
	{
		enemyTransform->Translate(enemy->m_CurrentDirection * enemy->m_Speed);

		if (enemy->m_CurrentDirection.x < 0)
		{
			if (enemyTransform->m_Position.x < minimumX)
			{
				enemy->m_CurrentDirection = { 1, enemy->m_CurrentDirection.y };
			}
		}
		else
		{
			if (enemyTransform->m_Position.x > maximumX)
			{
				enemy->m_CurrentDirection = { -1, enemy->m_CurrentDirection.y };
			}
		}
	}

	void FireBullet(SmallEnemy* enemy, float deltaTime)
	{

		enemy->m_TimeSinceLastShot -= deltaTime;

		if (enemy->m_TimeSinceLastShot <= 0.0f)
		{
			ECS::EntityID bullet = enemy->m_BulletPool->Rent(false);
			Vector2D enemyPosition = m_EntityManager->GetComponent<ECS::Transform>(enemy->GetID()).m_Position;

			ECS::SpriteRenderer* spriteRenderer = &m_EntityManager->GetComponent<ECS::SpriteRenderer>(bullet);
			ECS::Transform* bulletTransform = &m_EntityManager->GetComponent<ECS::Transform>(bullet);

			bulletTransform->m_Position = enemyPosition + enemy->m_BulletOffset;
			m_EntityManager->GetComponent<Bullet>(bullet).m_Speed = enemy->m_BulletSpeed;
			
			UpdateBulletSpriteDestRect(spriteRenderer, bulletTransform);
			
			m_EntityManager->SetEntityActive(bullet, true);

			enemy->m_TimeSinceLastShot = enemy->m_ShotTimer;
		}
	}

	void UpdateBulletSpriteDestRect(ECS::SpriteRenderer* spriteRenderer, ECS::Transform* bulletTransform)
	{
		spriteRenderer->m_DestRect.x = static_cast<int>(bulletTransform->m_Position.x);
		spriteRenderer->m_DestRect.y = static_cast<int>(bulletTransform->m_Position.y);
		spriteRenderer->m_DestRect.w = static_cast<int>(spriteRenderer->m_Width * bulletTransform->m_Scale.x);
		spriteRenderer->m_DestRect.h = static_cast<int>(spriteRenderer->m_Height * bulletTransform->m_Scale.y);
	}
};