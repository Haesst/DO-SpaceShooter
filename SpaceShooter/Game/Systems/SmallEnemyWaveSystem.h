#pragma once

#include "../../ECS/Base/EntityManager.h"
#include "../../ECS/Base/System.h"
#include "../Components/SmallEnemyWave.h"

struct SmallEnemyWaveSystem : ECS::System
{
	ECS::EntityManager* m_EntityManager;

	SmallEnemyWaveSystem()
	{
		AddComponentSignature<SmallEnemyWave>();
		m_EntityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{
		for (auto entity : m_Entities)
		{
			SmallEnemyWave* enemyWave = &m_EntityManager->GetComponent<SmallEnemyWave>(entity);

			for (auto waveEntity : enemyWave->m_CurrentWave)
			{
				if (!m_EntityManager->IsEntityActive(waveEntity))
				{
					enemyWave->m_CurrentWave.erase(
						std::remove(
							enemyWave->m_CurrentWave.begin(), 
							enemyWave->m_CurrentWave.end(), 
							waveEntity
						), 
						enemyWave->m_CurrentWave.end()
					);
				}
			}

			if (enemyWave->m_SpawningWave)
			{
				SpawnNewWave(enemyWave, currentState.m_DeltaTime);
				return;
			}

			if (enemyWave->m_CurrentWave.size() > 0)
			{
				return;
			}

			if (enemyWave->m_Waves.size() > 0)
			{
				enemyWave->m_SpawningWave = true;
			}
		}
	}

	void SpawnNewWave(SmallEnemyWave* enemyWave, float deltaTime)
	{
		if (enemyWave->m_CurrentTimeBetweenWaves > 0.0f)
		{
			enemyWave->m_CurrentTimeBetweenWaves -= deltaTime;
		}
		else
		{
			if (enemyWave->m_CurrentTimeBetweenSpawn > 0.0f)
			{
				enemyWave->m_CurrentTimeBetweenSpawn -= deltaTime;
			}
			else
			{
				auto entity = enemyWave->m_SmallEnemyPool->Rent(false);
				enemyWave->m_CurrentWave.push_back(entity);
				enemyWave->m_SpawnedThisWave++;

				ECS::Transform* enemyTransform = &m_EntityManager->GetComponent<ECS::Transform>(entity);
				ECS::SpriteRenderer* enemyRenderer = &m_EntityManager->GetComponent<ECS::SpriteRenderer>(entity);
				enemyTransform->m_Position = { 0.0f, 0.0f };
				UpdateEnemySpriteDestRect(enemyRenderer, enemyTransform);
				m_EntityManager->SetEntityActive(entity, true);

				enemyWave->m_CurrentTimeBetweenSpawn = enemyWave->m_TimeBetweenSpawns;

				if (enemyWave->m_SpawnedThisWave == enemyWave->m_Waves.front())
				{
					enemyWave->m_Waves.pop();

					enemyWave->m_CurrentTimeBetweenWaves = enemyWave->m_TimeBetweenWaves;
					enemyWave->m_CurrentTimeBetweenSpawn = 0.0f;
					enemyWave->m_SpawnedThisWave = 0;
					enemyWave->m_SpawningWave = false;
				}
			}
		}
	}

	void UpdateEnemySpriteDestRect(ECS::SpriteRenderer* spriteRenderer, ECS::Transform* bulletTransform)
	{
		spriteRenderer->m_DestRect.x = static_cast<int>(bulletTransform->m_Position.x);
		spriteRenderer->m_DestRect.y = static_cast<int>(bulletTransform->m_Position.y);
		spriteRenderer->m_DestRect.w = static_cast<int>(spriteRenderer->m_Width * bulletTransform->m_Scale.x);
		spriteRenderer->m_DestRect.h = static_cast<int>(spriteRenderer->m_Height * bulletTransform->m_Scale.y);
	}
};