#pragma once

#include "../../ECS/Base/Component.h"
#include "../EntityPool.h"

struct Bullet : public ECS::Component
{
	float m_Speed = 5.0f;
	float m_BulletLifeTime = 5.0f;
	float m_CurrentLifeTime = 5.0f;
	Vector2D m_Velocity = { 0.0f, 0.0f };

	EntityPool* m_OwningPool;

	Bullet(EntityPool* owningPool, Vector2D velocity)
		: m_OwningPool(owningPool), m_Velocity(velocity)
	{}

	void ReturnBulletToPool()
	{
		if (m_OwningPool != nullptr)
		{
			m_OwningPool->Unrent(m_EntityID);
		}
	}
};