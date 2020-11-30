#pragma once

#include "../../ECS/Base/Component.h"
#include "../EntityPool.h"

struct SmallEnemy : public ECS::Component
{
	float m_Speed = 2.0f;
	
	float m_BulletSpeed = 5.0f;
	float m_ShotTimer = 0.8f;
	float m_TimeSinceLastShot = 0.8f;
	Vector2D m_BulletOffset = { 0.0f, 50.0f };
	Vector2D m_CurrentDirection = { 1.0f, 0.0f };

	EntityPool* m_BulletPool;
	EntityPool* m_OwningPool;

	SmallEnemy(EntityPool* bulletPool, EntityPool* owningPool)
		: m_BulletPool(bulletPool), m_OwningPool(owningPool)
	{}
};