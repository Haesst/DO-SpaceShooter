#pragma once

#include "../../ECS/Base/EntityManager.h"
#include "../../ECS/Base/System.h"
#include "../../ECS/Components/Transform.h"
#include "../Components/Bullet.h"

struct BulletSystem : ECS::System
{
	ECS::EntityManager* entityManager;

	BulletSystem()
	{
		AddComponentSignature<Bullet>();
		entityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{
		std::vector<ECS::EntityID> killList;

		int currentWindowWidth = 0;
		int currentWindowHeight = 0;

		SDL_GetWindowSize(currentState.window, &currentWindowWidth, &currentWindowHeight);

		int minimumY = 0;

		for (auto entity : entities)
		{
			ECS::Transform* transform = &entityManager->GetComponent<ECS::Transform>(entity);
			Bullet* bullet = &entityManager->GetComponent<Bullet>(entity);

			float yPosition = transform->Position.y;

			if ((yPosition < minimumY) || (yPosition > currentWindowHeight))
			{
				bullet->owningPool->Unrent(entity);
				killList.push_back(entity);
			}

			transform->Translate(bullet->velocity * bullet->speed);
		}

		for (auto entity : killList)
		{
			entityManager->SetEntityActive(entity, false);
		}
	}
};