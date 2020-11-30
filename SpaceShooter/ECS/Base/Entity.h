#pragma once

#include "EntityManager.h"

namespace ECS
{
	struct Entity
	{
	private:
		EntityID id;
		EntityManager* m_EntityManager;

	public:
		Entity(const EntityID id, EntityManager* manager) : id(id), m_EntityManager(manager) {}
		~Entity() = default;

		const EntityID GetID() const
		{
			return id;
		}

		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			m_EntityManager->AddComponent<T>(id, std::forward<Args>(args)...);
		}

		template<typename T>
		void AddComponent(T& component)
		{
			m_EntityManager->AddComponent(id, component);
		}

		template<typename T>
		inline T& GetComponent()
		{
			return m_EntityManager->GetComponent<T>(id);
		}

		template<typename T>
		inline void RemoveComponent()
		{
			m_EntityManager->RemoveComponent<T>(id);
		}

		template<typename T>
		inline bool HasComponent()
		{
			return m_EntityManager->HasComponent<T>(id);
		}

		void Destroy()
		{
			m_EntityManager->DestroyEntity(id);
		}
	};
}