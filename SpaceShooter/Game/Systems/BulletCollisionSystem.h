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
	ECS::EntityManager* m_EntityManager;

	BulletCollisionSystem()
	{
		AddComponentSignature<BoxCollider2D>();
		m_EntityManager = &ECS::EntityManager::Get();
	}

	virtual void Update(GameState currentState) override
	{
		for (auto entity : m_Entities)
		{
			ECS::Transform* transform = &m_EntityManager->GetComponent<ECS::Transform>(entity);
			BoxCollider2D* collider = &m_EntityManager->GetComponent<BoxCollider2D>(entity);
			ECS::EntityTag entityTag = m_EntityManager->GetEntityTag(entity);

			collider->m_Box.x = transform->m_Position.x;
			collider->m_Box.y = transform->m_Position.y;
			collider->m_Box.w = collider->m_BaseWidth * transform->m_Scale.x;
			collider->m_Box.h = collider->m_BaseHeight * transform->m_Scale.y;
		}
	}
};