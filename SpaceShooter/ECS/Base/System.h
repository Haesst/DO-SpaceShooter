#pragma once

#include "Types.h"

#include "../../Engine/GameState.h"

namespace ECS
{
	class System
	{
	protected:
		friend class EntityManager;
		EntitySignature signature;
		std::set<EntityID> entities;

	public:
		System() = default;
		virtual ~System() = default;

		void RemoveEntity(const EntityID entity)
		{
			entities.erase(entity);
		}

		void AddEntity(const EntityID entity)
		{
			entities.insert(entity);
		}

		const EntitySignature GetSignature() const
		{
			return signature;
		}

		template<typename T>
		void AddComponentSignature()
		{
			signature.insert(ComponentType<T>());
		}

		const std::set<EntityID> GetEntities()
		{
			return entities;
		}

		virtual void Start() {}
		virtual void Update(GameState currentState) {}
		virtual void Render(GameState currentState) {}
		virtual void Destroy() {}
	};
}