#pragma once

#include <queue>
#include <map>
#include <memory>
#include <cassert>

#include "Types.h"
#include "System.h"
#include "Component.h"
#include "ComponentList.h"
#include "../Components/Transform.h"
// Todo: Add this to document -> think about other ways to keep ECS separated from engine
#include "../Engine/GameState.h"

namespace ECS
{
	enum class EntityTag
	{
		Untagged,
		Player,
		Enemy,
		PlayerBullet,
		EnemyBullet
	};

	class EntityManager
	{
	private:
		EntityID entityCount;
		std::queue<EntityID> availableEntityIDs;
		std::map<EntityID, std::shared_ptr<EntitySignature>> entitySignatures;
		std::map<SystemTypeID, std::shared_ptr<System>> registeredSystems;
		std::map<ComponentTypeID, std::shared_ptr<IComponentList>> componentArrays;
		std::map<EntityID, bool> activeEntities;
		std::map<EntityID, EntityTag> entityTags;

		static EntityManager* instance;

	private:

		template<typename T>
		void AddComponentList()
		{
			const ComponentTypeID componentType = ComponentType<T>();

			assert(componentArrays.find(componentType) == componentArrays.end() && "Component list already registered!");
			componentArrays[componentType] = std::move(std::make_shared<ComponentList<T>>());
		}

		void AddEntitySignature(const EntityID entity)
		{
			assert(entitySignatures.find(entity) == entitySignatures.end() && "Signature not found");
			entitySignatures[entity] = std::move(std::make_shared<EntitySignature>());
		}

		std::shared_ptr<EntitySignature> GetEntitySignature(const EntityID entity)
		{
			assert(entitySignatures.find(entity) != entitySignatures.end() && "Signature not found");
			return entitySignatures.at(entity);
		}

		void UpdateEntityTargetSystems(const EntityID entity)
		{
			for (auto& system : registeredSystems)
			{
				AddEntityToSystem(entity, system.second.get());
			}
		}

		void AddEntityToSystem(const EntityID entity, System* system)
		{
			if (BelongToSystem(entity, system->signature) && IsEntityActive(entity))
			{
				system->entities.insert(entity);
			}
			else
			{
				system->entities.erase(entity);
			}
		}

		bool BelongToSystem(const EntityID entity, const EntitySignature& systemSignature)
		{
			for (const auto componentType : systemSignature)
			{
				if (GetEntitySignature(entity)->count(componentType) == 0)
				{
					return false;
				}
			}
			
			return true;
		}

	public:
		EntityManager()
			: entityCount(0)
		{
			for (EntityID entity = 0u; entity < MAX_ENTITY_COUNT; entity++)
			{
				availableEntityIDs.push(entity);
			}
		}

		~EntityManager()
		{
			delete instance;
		}

		inline static EntityManager& Get()
		{
			if (instance == nullptr)
			{
				instance = new EntityManager();
			}

			return *instance;
		}

		void Update(GameState currentState)
		{
			for (auto& system : registeredSystems)
			{
				system.second->Update(currentState);
			}
		}

		void Render(GameState currentState)
		{
			for (auto& system : registeredSystems)
			{
				system.second->Render(currentState);
			}
		}

		const EntityID AddNewEntity()
		{
			const EntityID entityID = availableEntityIDs.front();
			AddEntitySignature(entityID);

			AddComponent<Transform>(entityID);

			availableEntityIDs.pop();
			entityCount++;

			activeEntities[entityID] = true;
			entityTags[entityID] = EntityTag::Untagged;
			
			return entityID;
		}

		const EntityID AddNewEntity(float x, float y)
		{
			const EntityID entityID = availableEntityIDs.front();
			AddEntitySignature(entityID);

			AddComponent<Transform>(entityID, x, y);

			availableEntityIDs.pop();
			entityCount++;

			activeEntities[entityID] = true;
			entityTags[entityID] = EntityTag::Untagged;

			return entityID;
		}

		void DestroyEntity(const EntityID entity)
		{
			AssertEntityNotOutOfRange(entity);

			entitySignatures.erase(entity);

			for (auto& array : componentArrays)
			{
				array.second->Erase(entity);
			}

			for (auto& system : registeredSystems)
			{
				system.second->RemoveEntity(entity);
			}

			activeEntities.erase(entity);
			entityTags.erase(entity);

			entityCount--;
			availableEntityIDs.push(entity);
		}

		void SetEntityActive(const EntityID entity, const bool active)
		{
			activeEntities[entity] = active;
			UpdateEntityTargetSystems(entity);
		}

		bool IsEntityActive(const EntityID entity)
		{
			return activeEntities[entity];
		}

		void SetEntityTag(const EntityID entity, const EntityTag tag)
		{
			entityTags[entity] = tag;
		}

		EntityTag GetEntityTag(const EntityID entity)
		{
			return entityTags[entity];
		}

		template<typename T, typename... Args>
		void AddComponent(const EntityID entity, Args&&... args)
		{
			AssertEntityNotOutOfRange(entity);
			assert(entitySignatures[entity]->size() < MAX_COMPONENT_COUNT && "Component count limit reached!");

			T component(std::forward<Args>(args)...);
			component.entityID = entity;
			GetEntitySignature(entity)->insert(ComponentType<T>());
			GetComponentList<T>()->Insert(component);

			const ComponentTypeID componentType = ComponentType<T>();
			entitySignatures.at(entity)->insert(componentType);
			UpdateEntityTargetSystems(entity);
		}

		template<typename T>
		void RemoveComponent(const EntityID entity)
		{
			AssertEntityNotOutOfRange(entity);

			const ComponentTypeID componentType = ComponentType<T>();
			entitySignatures.at(entity)->erase(componentType);
			GetComponentList<T>()->Erase(entity);
			UpdateEntityTargetSystems(entity);
		}

		template<typename T>
		T& GetComponent(const EntityID entity)
		{
			AssertEntityNotOutOfRange(entity);

			const ComponentTypeID componentType = ComponentType<T>();
			return GetComponentList<T>()->Get(entity);
		}

		template<typename T>
		std::shared_ptr<ComponentList<T>> GetComponentList()
		{
			const ComponentTypeID componentType = ComponentType<T>();

			if (componentArrays.count(componentType) == 0)
			{
				AddComponentList<T>();
			}

			return std::static_pointer_cast<ComponentList<T>>(componentArrays.at(componentType));
		}

		template<typename T>
		const bool HasComponent(const EntityID entity)
		{
			AssertEntityNotOutOfRange(entity);

			return (GetEntitySignature(entity)->count(ComponentType<T>()) > 0);
		}

		void AssertEntityNotOutOfRange(const EntityID entity)
		{
			assert(entity < MAX_ENTITY_COUNT && "EntityID out of range");
		}

		template<typename T>
		void RegisterSystem()
		{
			const SystemTypeID systemType = SystemType<T>();
			assert(registeredSystems.count(systemType) == 0 && "System already registered");
			auto system = std::make_shared<T>();

			// Add entity to new system
			for (EntityID entity = 0; entity < entityCount; entity++)
			{
				AddEntityToSystem(entity, system.get());
			}

			system->Start();
			registeredSystems[systemType] = std::move(system);
		}

		template<typename T>
		std::shared_ptr<System> GetSystem()
		{
			const SystemTypeID systemType = SystemType<T>();
			assert(registeredSystems.count(systemType) != 0 && "System not registered!");
			return registeredSystems[systemType];
		}

		template<typename T>
		void UnregisterSystem()
		{
			const SystemTypeID systemType = SystemType<T>();
			assert(registeredSystems.count(systemType) != 0 && "System not registered!");
			registeredSystems.erase(systemType);
		}
	};
}