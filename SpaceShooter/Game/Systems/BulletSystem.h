#pragma once

#include "../../ECS/Base/EntityManager.h"
#include "../../ECS/Base/System.h"
#include "../../ECS/Components/Transform.h"
#include "../Components/Bullet.h"

struct BulletSystem : ECS::System
{
	ECS::EntityManager* m_EntityManager;

	BulletSystem()
	{
		AddComponentSignature<Bullet>();
		m_EntityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{
		std::vector<ECS::EntityID> killList;

		int currentWindowWidth = 0;
		int currentWindowHeight = 0;

		SDL_GetWindowSize(currentState.m_Window, &currentWindowWidth, &currentWindowHeight);

		int minimumY = 0;

		for (auto entity : m_Entities)
		{
			ECS::Transform* transform = &m_EntityManager->GetComponent<ECS::Transform>(entity);
			Bullet* bullet = &m_EntityManager->GetComponent<Bullet>(entity);

			float yPosition = transform->m_Position.y;

			if ((yPosition < minimumY) || (yPosition > currentWindowHeight))
			{
				bullet->m_OwningPool->Unrent(entity);
				killList.push_back(entity);
			}

			transform->Translate(bullet->m_Velocity * bullet->m_Speed);
		}

		for (auto entity : killList)
		{
			m_EntityManager->SetEntityActive(entity, false);
		}
	}
};