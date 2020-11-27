#pragma once

#include "../../ECS/Base/Component.h"
#include "../EntityPool.h"

struct Bullet : public ECS::Component
{
	float speed = 5.0f;
	float bulletLifeTime = 5.0f;
	float currentLifeTime = 5.0f;
	Vector2D velocity = { 0.0f, 0.0f };

	EntityPool* owningPool;

	Bullet(EntityPool* owningPool, Vector2D velocity)
		: owningPool(owningPool), velocity(velocity)
	{}

	void ReturnBulletToPool()
	{
		if (owningPool != nullptr)
		{
			owningPool->Unrent(entityID);
		}
	}
};