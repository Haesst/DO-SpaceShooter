#pragma once

#include "EntityManager.h"

namespace ECS
{
	struct Entity // Todo: Add something about this to document -> Wrapper class
	{
	private:
		EntityID id;
		EntityManager* manager;

	public:
		Entity(const EntityID id, EntityManager* manager) : id(id), manager(manager) {}
		~Entity() = default;

		const EntityID GetID() const
		{
			return id;
		}

		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			manager->AddComponent<T>(id, std::forward<Args>(args)...);
		}

		template<typename T>
		void AddComponent(T& component)
		{
			manager->AddComponent(id, component);
		}

		template<typename T>
		inline T& GetComponent()
		{
			return manager->GetComponent<T>(id);
		}

		template<typename T>
		inline void RemoveComponent()
		{
			manager->RemoveComponent<T>(id);
		}

		template<typename T>
		inline bool HasComponent()
		{
			return manager->HasComponent<T>(id);
		}

		void Destroy()
		{
			manager->DestroyEntity(id);
		}
	};
}