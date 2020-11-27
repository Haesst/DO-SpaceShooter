#pragma once

#include "Types.h"

namespace ECS
{
	struct Component
	{
	protected:
		friend class EntityManager;
		EntityID entityID;

	public:
		Component() : entityID() {}
		virtual ~Component() {}
		inline const EntityID GetID() const { return entityID; }
	};
}