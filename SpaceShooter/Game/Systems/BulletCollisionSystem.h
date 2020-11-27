#pragma once

#include "../../ECS/Base/EntityManager.h"
#include "../../ECS/Base/System.h"
#include "../../ECS/Components/Transform.h"
#include "../Components/BoxCollider2D.h"
#include "../Components/Bullet.h"
#include "../Components/Player.h"
#include "../Components/SmallEnemy.h"

struct BulletCollisionSystem : ECS::System
{
	ECS::EntityManager* entityManager;

	BulletCollisionSystem()
	{
		AddComponentSignature<BoxCollider2D>();
		entityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{
		for (auto entity : entities)
		{
			ECS::Transform* transform = &entityManager->GetComponent<ECS::Transform>(entity);
			BoxCollider2D* collider = &entityManager->GetComponent<BoxCollider2D>(entity);
			ECS::EntityTag entityTag = entityManager->GetEntityTag(entity);

			collider->Box.x = transform->Position.x;
			collider->Box.y = transform->Position.y;
			collider->Box.w = collider->baseWidth * transform->Scale.x;
			collider->Box.h = collider->baseHeight * transform->Scale.y;
		}
	}
};