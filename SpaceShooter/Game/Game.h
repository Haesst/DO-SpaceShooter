#pragma once

#include "EntityPool.h"
#include "../Engine/GameState.h"

class Engine;
class EntityManager;

class Game
{
private:
	EntityPool* m_SmallEnemyPool;
	EntityPool* m_SmallEnemyBulletPool;
	EntityPool* m_PlayerBulletPool;

	GameState m_GameState;

	AssetManager* m_AssetManager;
	ECS::EntityManager* m_EntityManager;

	std::string m_PlayerBulletTextureID = "PlayerBullet";
	std::string m_EnemyBulletTextureID = "EnemyBullet";
	std::string m_PlayerTextureID = "PlayerShip";
	std::string m_SmallEnemyTextureID = "SmallEnemy";

	std::string m_PlayerBulletTexturePath = "Sprites/PlayerBullet.png";
	std::string m_EnemyBulletTexturePath = "Sprites/SmallEnemyBullet.png";
	std::string m_PlayerTexturePath = "Sprites/PlayerShip.png";
	std::string m_SmallEnemyTexturePath = "Sprites/SmallEnemy.png";

	Vector2D m_PlayerBulletVelocity = { 0.0f, -1.0f };
	Vector2D m_EnemyBulletVelocity = { 0.0f, 1.0f };
	Vector2D m_PlayerStartPosition = { 600.0f, 600.0f };

private:
	void LoadAssets();
	void RegisterSystems();
	void CreateEntities();
	void CreateBulletPools();
	void CreateBulletPool(int initAmount, int expandBy, ECS::EntityTag tag, EntityPool*& pool, std::string textureID, Vector2D initVelocity);
	void CreatePlayer();
	void CreateEnemyPool(int initAmount, int expandBy);
	void CreateEnemyWaves();

	void HandleCollisions();
	bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB) const;
	void Collide(const ECS::EntityID entity, const ECS::EntityID otherEntity, std::vector<ECS::EntityID>* killList);

public:
	void Init(Engine* engine);
	void Quit();

	void Run();
};
