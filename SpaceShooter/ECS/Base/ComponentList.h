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
		std::vector<T> m_Data;

	public:
		ComponentList() = default;
		~ComponentList() = default;

		void Insert(const T& component)
		{
			bool idExists = false;

			for (auto c : m_Data)
			{
				if (c.GetID() == component.GetID())
				{
					idExists = true;
				}
			}

			if (!idExists)
			{
				m_Data.push_back(component);
			}
		}

		T& Get(const EntityID entity)
		{
			auto comp = std::find_if(m_Data.begin(), m_Data.end(), [&](const T& c) { return c.GetID() == entity; });
			assert(comp != m_Data.end() && "Trying to get non-existing component!");
			return *comp;
		}

		void Erase(const EntityID entity) override final
		{
			auto comp = std::find_if(m_Data.begin(), m_Data.end(), [&](const T& c) { return c.GetID() == entity; });

			if (comp != m_Data.end())
			{
				m_Data.erase(comp);
			}
		}
	};
}