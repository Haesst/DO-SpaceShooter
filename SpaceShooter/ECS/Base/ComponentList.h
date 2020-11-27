#pragma once

#include <algorithm>

#include "Types.h"

namespace ECS
{
	class IComponentList
	{
	public:
		IComponentList() = default;
		~IComponentList() = default;
		virtual void Erase(const EntityID entity) {}
	};

	template<typename T>
	class ComponentList : public IComponentList
	{
	public:
		std::vector<T> data; // Todo: Add this to document -> Maybe change to an actual array in order to not allocate memory on the heap?

	public:
		ComponentList() = default;
		~ComponentList() = default;

		void Insert(const T& component)
		{
			bool idExists = false;

			for (auto c : data)
			{
				if (c.GetID() == component.GetID())
				{
					idExists = true;
				}
			}

			if (!idExists)
			{
				data.push_back(component);
			}
		}

		T& Get(const EntityID entity)
		{
			auto comp = std::find_if(data.begin(), data.end(), [&](const T& c) { return c.GetID() == entity; });
			assert(comp != data.end() && "Trying to get non-existing component!");
			return *comp;
		}

		void Erase(const EntityID entity) override final
		{
			auto comp = std::find_if(data.begin(), data.end(), [&](const T& c) { return c.GetID() == entity; });

			if (comp != data.end())
			{
				data.erase(comp);
			}
		}
	};
}