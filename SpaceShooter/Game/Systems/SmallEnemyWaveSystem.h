#pragma once

#include "../../ECS/Base/EntityManager.h"
#include "../../ECS/Base/System.h"
#include "../Components/SmallEnemyWave.h"

struct SmallEnemyWaveSystem : ECS::System
{
	ECS::EntityManager* entityManager;

	SmallEnemyWaveSystem()
	{
		AddComponentSignature<SmallEnemyWave>();
		entityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{
		for (auto entity : entities)
		{
			SmallEnemyWave* enemyWave = &entityManager->GetComponent<SmallEnemyWave>(entity);

			for (auto waveEntity : enemyWave->currentWave)
			{
				if (!entityManager->IsEntityActive(waveEntity))
				{
					enemyWave->currentWave.erase(
						std::remove(
							enemyWave->currentWave.begin(), 
							enemyWave->currentWave.end(), 
							waveEntity
						), 
						enemyWave->currentWave.end()
					);
				}
			}

			if (enemyWave->spawningWave)
			{
				SpawnNewWave(enemyWave, currentState.deltaTime);
				return;
			}

			if (enemyWave->currentWave.size() > 0)
			{
				return;
			}

			if (enemyWave->waves.size() > 0)
			{
				enemyWave->spawningWave = true;
			}
		}
	}

	void SpawnNewWave(SmallEnemyWave* enemyWave, float deltaTime)
	{
		if (enemyWave->currentTimeBetweenWaves > 0.0f)
		{
			enemyWave->currentTimeBetweenWaves -= deltaTime;
		}
		else
		{
			if (enemyWave->currentTimeBetweenSpawn > 0.0f)
			{
				enemyWave->currentTimeBetweenSpawn -= deltaTime;
			}
			else
			{
				auto entity = enemyWave->smallEnemyPool->Rent(false);
				enemyWave->currentWave.push_back(entity);
				enemyWave->spawnedThisWave++;

				ECS::Transform* enemyTransform = &entityManager->GetComponent<ECS::Transform>(entity);
				ECS::SpriteRenderer* enemyRenderer = &entityManager->GetComponent<ECS::SpriteRenderer>(entity);
				enemyTransform->Position = { 0.0f, 0.0f };
				UpdateEnemySpriteDestRect(enemyRenderer, enemyTransform);
				entityManager->SetEntityActive(entity, true);

				enemyWave->currentTimeBetweenSpawn = enemyWave->timeBetweenSpawns;

				if (enemyWave->spawnedThisWave == enemyWave->waves.front())
				{
					enemyWave->waves.pop();

					enemyWave->currentTimeBetweenWaves = enemyWave->timeBetweenWaves;
					enemyWave->currentTimeBetweenSpawn = 0.0f;
					enemyWave->spawnedThisWave = 0;
					enemyWave->spawningWave = false;
				}
			}
		}
	}

	void UpdateEnemySpriteDestRect(ECS::SpriteRenderer* spriteRenderer, ECS::Transform* bulletTransform)
	{
		spriteRenderer->destRect.x = static_cast<int>(bulletTransform->Position.x);
		spriteRenderer->destRect.y = static_cast<int>(bulletTransform->Position.y);
		spriteRenderer->destRect.w = static_cast<int>(spriteRenderer->width * bulletTransform->Scale.x);
		spriteRenderer->destRect.h = static_cast<int>(spriteRenderer->height * bulletTransform->Scale.y);
	}
};