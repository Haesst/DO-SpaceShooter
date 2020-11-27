#pragma once

#include "../../ECS/Base/Component.h"
#include "../EntityPool.h"

struct SmallEnemyWave : public ECS::Component
{
	EntityPool* smallEnemyPool;

	bool spawningWave = false;
	int spawnedThisWave = 0;

	float timeBetweenSpawns = 0.7f;
	float currentTimeBetweenSpawn = 0.0f;

	float timeBetweenWaves = 1.5f;
	float currentTimeBetweenWaves = 1.5f;

	std::vector<ECS::EntityID> currentWave;
	std::queue<int> waves;

	SmallEnemyWave(EntityPool* enemyPool) : smallEnemyPool(enemyPool) {}
	~SmallEnemyWave() = default;
};