#pragma once

#include <set>

namespace ECS
{
	class System;
	struct Component;

	// Constants
	const size_t MAX_ENTITY_COUNT = 5000;
	const size_t MAX_COMPONENT_COUNT = 32;

	// Custom Types
	using EntityID = size_t;
	using SystemTypeID = size_t;
	using ComponentTypeID = size_t;
	using EntitySignature = std::set<ComponentTypeID>;

	// Returns component runtime type id
	inline static const ComponentTypeID GetRuntimeComponentTypeID()
	{
		static ComponentTypeID typeID = 0u;
		return typeID++;
	}

	// Returns component runtime type id
	inline static const SystemTypeID GetRuntimeSystemTypeID()
	{
		static SystemTypeID typeID = 0u;
		return typeID++;
	}

	// Attach type id to component class and return it
	template<typename T>
	inline static const ComponentTypeID ComponentType() noexcept
	{
		static_assert((std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value), "INVALID TEMPLATE TYPE");
		static const ComponentTypeID typeID = GetRuntimeComponentTypeID();
		return typeID;
	}

	// Attach type id to system class and return it
	template<typename T>
	inline static const SystemTypeID SystemType() noexcept
	{
		static_assert((std::is_base_of<System, T>::value && !std::is_same<System, T>::value), "INVALID TEMPLATE TYPE");
		static const SystemTypeID typeID = GetRuntimeSystemTypeID();

		return typeID;
	}
}