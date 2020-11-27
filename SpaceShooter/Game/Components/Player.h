#pragma once

#include "../../ECS/Base/Component.h"
#include "../../ECS/Base/EntityManager.h"

struct Player : public ECS::Component
{
	int health = 3;
	float speed = 4.0f;

	Vector2D bulletOffset = { 0.0f, -50.0f };
	float timeBetweenBullets = 1.8f;
	float currentTimeBetweenBullets = 0.0f;
	float bulletSpeed = 10.0f;

	EntityPool* bulletPool;

	void SetPosition(float x, float y)
	{
		ECS::Transform* transform = &ECS::EntityManager::Get().GetComponent<ECS::Transform>(entityID);

		transform->Position.x = x;
		transform->Position.y = y;
	}

	void HitPlayer()
	{
		health--;
	}

	bool PlayerAlive()
	{
		return health > 0;
	}
};