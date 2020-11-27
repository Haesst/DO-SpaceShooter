#pragma once

#include <deque>
#include <iostream>

#include "../ECS/Base/Types.h"
#include "../ECS/Base/EntityManager.h"

#include "../ECS/Components/SpriteRenderer.h"

class EntityPool
{
private:
	int entityCount;
	std::deque<ECS::EntityID> unusedEntities;
	std::vector<ECS::EntityID> allEntitites;
	unsigned int expandBy;

public:
	EntityPool(unsigned int initSize, unsigned int expandBy = 1)
		: entityCount(0u), expandBy(expandBy)
	{
		for (int i = 0; i < initSize; i++)
		{
			ECS::EntityID entity = ECS::EntityManager::Get().AddNewEntity();
			unusedEntities.push_back(entity);
			allEntitites.push_back(entity);
			ECS::EntityManager::Get().SetEntityActive(entity, false);
		}
	}

	template<typename T, typename... Args>
	void AddComponentToPool(Args&&... args)
	{
		for (auto entity : unusedEntities)
		{
			ECS::EntityManager::Get().AddComponent<T>(entity, std::forward<Args>(args)...);
		}
	}

	template<typename T, typename... Args>
	void RemoveComponentFromPool(Args&&... args)
	{
		for (auto entity : unusedEntities)
		{
			ECS::EntityManager::Get().RemoveComponent<T>(entity, std::forward<Args>(args)...);
		}
	}

	void SetPoolEntitiesTag(ECS::EntityTag tag)
	{
		for (auto entity : unusedEntities)
		{
			ECS::EntityManager::Get().SetEntityTag(entity, tag);
		}
	}

	ECS::EntityID Rent(bool returnActive = true)
	{
		if (unusedEntities.size() <= 0)
		{
			std::cerr << "Entity pool to small." << std::endl; // very bad way to quickly skip doing an expand for now
		}

		ECS::EntityID entity = unusedEntities.front();
		unusedEntities.pop_front();

		ECS::EntityManager::Get().SetEntityActive(entity, returnActive);
		return entity;
	}

	void Unrent(ECS::EntityID entity)
	{
		unusedEntities.push_back(entity);
	}
};