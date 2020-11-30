#pragma once

#include "../../ECS/Base/Component.h"
#include "../../ECS/Base/EntityManager.h"

struct Player : public ECS::Component
{
	int m_Health = 3;
	float m_Speed = 4.0f;

	Vector2D m_BulletOffset = { 0.0f, -50.0f };
	float m_TimeBetweenBullets = 1.8f;
	float m_CurrentTimeBetweenBullets = 0.0f;
	float m_BulletSpeed = 10.0f;

	EntityPool* m_BulletPool;

	void SetPosition(float x, float y)
	{
		ECS::Transform* transform = &ECS::EntityManager::Get().GetComponent<ECS::Transform>(m_EntityID);

		transform->m_Position.x = x;
		transform->m_Position.y = y;
	}

	void HitPlayer()
	{
		m_Health--;
	}

	bool PlayerAlive()
	{
		return m_Health > 0;
	}
};