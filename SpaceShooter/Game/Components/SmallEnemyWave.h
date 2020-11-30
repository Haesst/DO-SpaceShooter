#pragma once

#include "../../ECS/Base/Component.h"
#include "../EntityPool.h"

struct SmallEnemyWave : public ECS::Component
{
	EntityPool* m_SmallEnemyPool;

	bool m_SpawningWave = false;
	int m_SpawnedThisWave = 0;

	float m_TimeBetweenSpawns = 0.7f;
	float m_CurrentTimeBetweenSpawn = 0.0f;

	float m_TimeBetweenWaves = 1.5f;
	float m_CurrentTimeBetweenWaves = 1.5f;

	std::vector<ECS::EntityID> m_CurrentWave;
	std::queue<int> m_Waves;

	SmallEnemyWave(EntityPool* enemyPool) : m_SmallEnemyPool(enemyPool) {}
	~SmallEnemyWave() = default;
};