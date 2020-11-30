#pragma once

#include <deque>
#include <iostream>

#include "../ECS/Base/Types.h"
#include "../ECS/Base/EntityManager.h"

#include "../ECS/Components/SpriteRenderer.h"

class EntityPool
{
private:
	int m_EntityCount;
	std::deque<ECS::EntityID> m_UnusedEntities;
	std::vector<ECS::EntityID> m_AllEntitites;
	unsigned int m_ExpandBy;

public:
	EntityPool(unsigned int initSize, unsigned int expandBy = 1)
		: m_EntityCount(0u), m_ExpandBy(expandBy)
	{
		for (int i = 0; i < initSize; i++)
		{
			ECS::EntityID entity = ECS::EntityManager::Get().AddNewEntity();
			m_UnusedEntities.push_back(entity);
			m_AllEntitites.push_back(entity);
			ECS::EntityManager::Get().SetEntityActive(entity, false);
		}
	}

	template<typename T, typename... Args>
	void AddComponentToPool(Args&&... args)
	{
		for (auto entity : m_UnusedEntities)
		{
			ECS::EntityManager::Get().AddComponent<T>(entity, std::forward<Args>(args)...);
		}
	}

	template<typename T, typename... Args>
	void RemoveComponentFromPool(Args&&... args)
	{
		for (auto entity : m_UnusedEntities)
		{
			ECS::EntityManager::Get().RemoveComponent<T>(entity, std::forward<Args>(args)...);
		}
	}

	void SetPoolEntitiesTag(ECS::EntityTag tag)
	{
		for (auto entity : m_UnusedEntities)
		{
			ECS::EntityManager::Get().SetEntityTag(entity, tag);
		}
	}

	ECS::EntityID Rent(bool returnActive = true)
	{
		if (m_UnusedEntities.size() <= 0)
		{
			std::cerr << "Entity pool to small." << std::endl; // very bad way to quickly skip doing an expand for now
		}

		ECS::EntityID entity = m_UnusedEntities.front();
		m_UnusedEntities.pop_front();

		ECS::EntityManager::Get().SetEntityActive(entity, returnActive);
		return entity;
	}

	void Unrent(ECS::EntityID entity)
	{
		m_UnusedEntities.push_back(entity);
	}
};