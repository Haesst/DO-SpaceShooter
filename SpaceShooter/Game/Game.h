#pragma once

#include "EntityPool.h"
#include "../Engine/GameState.h"

class Engine;
class EntityManager;

class Game
{
private:
	EntityPool* smallEnemyPool;
	EntityPool* smallEnemyBulletPool;
	EntityPool* playerBulletPool;

	GameState gameState;

	AssetManager* assetManager;
	ECS::EntityManager* entityManager;

	std::string playerBulletTextureID = "PlayerBullet";
	std::string enemyBulletTextureID = "EnemyBullet";
	std::string playerTextureID = "PlayerShip";
	std::string smallEnemyTextureID = "SmallEnemy";

	std::string playerBulletTexturePath = "Sprites/PlayerBullet.png";
	std::string enemyBulletTexturePath = "Sprites/SmallEnemyBullet.png";
	std::string playerTexturePath = "Sprites/PlayerShip.png";
	std::string smallEnemyTexturePath = "Sprites/SmallEnemy.png";

	Vector2D playerBulletVelocity = { 0.0f, -1.0f };
	Vector2D enemyBulletVelocity = { 0.0f, 1.0f };
	Vector2D playerStartPosition = { 600.0f, 600.0f };

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
