#pragma once

#include "../../ECS/Base/Component.h"
#include "../EntityPool.h"

struct SmallEnemy : public ECS::Component
{
	float speed = 2.0f;
	
	float bulletSpeed = 5.0f;
	float shotTimer = 0.8f;
	float timeSinceLastShot = 0.8f;
	Vector2D bulletOffset = { 0.0f, 50.0f };
	Vector2D currentDirection = { 1.0f, 0.0f };

	EntityPool* bulletPool;
	EntityPool* owningPool;

	SmallEnemy(EntityPool* bulletPool, EntityPool* owningPool)
		: bulletPool(bulletPool), owningPool(owningPool)
	{}
};