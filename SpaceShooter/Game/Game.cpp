#include "Game.h"

#include "../Engine/Engine.h"
#include "../ECS/Systems/SpriteRenderSystem.h"
#include "Systems/BulletCollisionSystem.h"
#include "Systems/BulletSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/SmallEnemySystem.h"
#include "Systems/SmallEnemyWaveSystem.h"
#include "Components/BoxCollider2D.h"
#include "Components/Bullet.h"

// Todo: Create a better collisionsystem <- not in every update but in a standalone physics system
// Todo: Try to fix so entitypools can expand (need to create prefabs)
// Todo: Clean code

const int FRAMERATE = 120;
const int MILLISECONDTOSECOND = 1000;

void Game::Init(Engine* engine)
{
	// Init gamestate
	gameState.engine = engine;
	gameState.renderer = engine->GetRenderer();
	gameState.window = engine->GetWindow();

	assetManager = engine->GetAssetManager();
	entityManager = &ECS::EntityManager::Get();

	LoadAssets();
	RegisterSystems();
	CreateEntities();
}

void Game::Quit()
{
	delete smallEnemyPool;
	delete smallEnemyBulletPool;
	delete playerBulletPool;
}

void Game::LoadAssets()
{
	assetManager->Init(gameState.engine);
	assetManager->LoadTexture(playerTextureID, playerTexturePath);
	assetManager->LoadTexture(smallEnemyTextureID, smallEnemyTexturePath);
	assetManager->LoadTexture(enemyBulletTextureID, enemyBulletTexturePath);
	assetManager->LoadTexture(playerBulletTextureID, playerBulletTexturePath);
}

void Game::RegisterSystems()
{
	entityManager->RegisterSystem<ECS::SpriteRenderSystem>();
	entityManager->RegisterSystem<PlayerSystem>();
	entityManager->RegisterSystem<SmallEnemySystem>();
	entityManager->RegisterSystem<BulletSystem>();
	entityManager->RegisterSystem<BulletCollisionSystem>();
	entityManager->RegisterSystem<SmallEnemyWaveSystem>();
}

void Game::CreateEntities()
{
	CreateBulletPools();
	CreatePlayer();
	CreateEnemyPool(60, 1);
	CreateEnemyWaves();
}

void Game::CreateBulletPools()
{
	CreateBulletPool(300, 1, ECS::EntityTag::PlayerBullet, playerBulletPool, playerBulletTextureID, playerBulletVelocity);
	CreateBulletPool(300, 1, ECS::EntityTag::EnemyBullet, smallEnemyBulletPool, enemyBulletTextureID, enemyBulletVelocity);
}

void Game::CreateBulletPool(int initAmount, int expandBy, ECS::EntityTag tag, EntityPool*& pool, std::string textureID, Vector2D initVelocity)
{
	pool = new EntityPool(initAmount, expandBy);
	SDL_Texture* bulletTexture = assetManager->GetTexture(textureID);
	pool->AddComponentToPool<ECS::SpriteRenderer>(gameState.engine->GetRenderer(), bulletTexture);
	int bulletColliderWidth = 0;
	int bulletColliderHeight = 0;
	SDL_QueryTexture(bulletTexture, nullptr, nullptr, &bulletColliderWidth, &bulletColliderHeight);
	pool->AddComponentToPool<BoxCollider2D>(bulletColliderWidth, bulletColliderHeight);
	pool->AddComponentToPool<Bullet>(pool, initVelocity);
	pool->SetPoolEntitiesTag(tag);
}

void Game::CreatePlayer()
{
	ECS::EntityID id = entityManager->AddNewEntity(playerStartPosition.x, playerStartPosition.y);
	entityManager->AddComponent<Player>(id);
	SDL_Texture* playerTexture = assetManager->GetTexture(playerTextureID);
	entityManager->AddComponent<ECS::SpriteRenderer>(id, gameState.engine->GetRenderer(), playerTexture);
	int playerColliderWidth = 0;
	int playerColliderHeight = 0;
	SDL_QueryTexture(playerTexture, nullptr, nullptr, &playerColliderWidth, &playerColliderHeight);
	entityManager->AddComponent<BoxCollider2D>(id, playerColliderWidth, playerColliderHeight);
	entityManager->SetEntityActive(id, true);
	entityManager->SetEntityTag(id, ECS::EntityTag::Player);
	entityManager->GetComponent<Player>(id).bulletPool = playerBulletPool;
}

void Game::CreateEnemyPool(int initAmount, int expandBy)
{
	smallEnemyPool = new EntityPool(60, 1);
	SDL_Texture* enemyTexture = assetManager->GetTexture(smallEnemyTextureID);
	smallEnemyPool->AddComponentToPool<ECS::SpriteRenderer>(gameState.engine->GetRenderer(), enemyTexture);
	int enemyColliderWidth = 0;
	int enemyColliderHeight = 0;
	SDL_QueryTexture(enemyTexture, nullptr, nullptr, &enemyColliderWidth, &enemyColliderHeight);
	smallEnemyPool->AddComponentToPool<BoxCollider2D>(enemyColliderWidth, enemyColliderHeight);
	smallEnemyPool->AddComponentToPool<SmallEnemy>(smallEnemyBulletPool, smallEnemyPool);
	smallEnemyPool->SetPoolEntitiesTag(ECS::EntityTag::Enemy);
}

void Game::CreateEnemyWaves()
{
	ECS::EntityID enemyWaveID = ECS::EntityManager::Get().AddNewEntity();
	entityManager->AddComponent<SmallEnemyWave>(enemyWaveID, smallEnemyPool);
	SmallEnemyWave* enemyWave = &ECS::EntityManager::Get().GetComponent<SmallEnemyWave>(enemyWaveID);

	enemyWave->waves.push(3);
	enemyWave->waves.push(6);
	enemyWave->waves.push(8);
	enemyWave->waves.push(14);
	enemyWave->waves.push(18);
}

void Game::HandleCollisions()
{
	ECS::System* bulletCollisionSystem = entityManager->GetSystem<BulletCollisionSystem>().get();

	std::set<ECS::EntityID> entities;
	std::vector<ECS::EntityID> killList;

	if (bulletCollisionSystem != nullptr)
	{
		entities = bulletCollisionSystem->GetEntities();
	}


	for (ECS::EntityID entity : entities)
	{
		ECS::EntityTag entityTag = entityManager->GetEntityTag(entity);

		if (entityTag == ECS::EntityTag::EnemyBullet || entityTag == ECS::EntityTag::PlayerBullet)
		{
			continue;
		}

		for (ECS::EntityID otherEntity : entities)
		{
			if (entity == otherEntity)
			{
				continue;
			}

			ECS::EntityTag otherTag = entityManager->GetEntityTag(otherEntity);

			if ((entityTag == ECS::EntityTag::Player && otherTag == ECS::EntityTag::EnemyBullet) ||
				(entityTag == ECS::EntityTag::Enemy && otherTag == ECS::EntityTag::PlayerBullet))
			{
				BoxCollider2D collider = entityManager->GetComponent<BoxCollider2D>(entity);
				BoxCollider2D otherCollider = entityManager->GetComponent<BoxCollider2D>(otherEntity);

				if (AABB(collider.Box, otherCollider.Box))
				{
					Collide(entity, otherEntity, &killList);
				}
			}
		}
	}

	for (ECS::EntityID entity : killList)
	{
		entityManager->SetEntityActive(entity, false);
	}
}

bool Game::AABB(const SDL_Rect& rectA, const SDL_Rect& rectB) const
{
	return (rectA.x + rectA.w >= rectB.x && rectB.x + rectB.w >= rectA.x &&
		rectA.y + rectA.h >= rectB.y && rectB.y + rectB.h >= rectA.y);
}

void Game::Collide(const ECS::EntityID entity, const ECS::EntityID otherEntity, std::vector<ECS::EntityID>* killList)
{
	ECS::EntityTag entityTag = entityManager->GetEntityTag(entity);
	ECS::EntityTag otherTag = entityManager->GetEntityTag(otherEntity);

	if (entityTag == ECS::EntityTag::Player)
	{
		if (otherTag == ECS::EntityTag::EnemyBullet)
		{
			killList->push_back(otherEntity);

			Player* player = &entityManager->GetComponent<Player>(entity);
			player->HitPlayer();

			if (!player->PlayerAlive())
			{
				killList->push_back(entity);
			}
		}
	}
	else if (entityTag == ECS::EntityTag::Enemy)
	{
		if (otherTag == ECS::EntityTag::PlayerBullet)
		{
			killList->push_back(entity);
			killList->push_back(otherEntity);
		}
	}
}

void Game::Run()
{
	int timerFPS;
	Uint64 currentFrameCounter = SDL_GetPerformanceCounter();
	Uint64 lastFrameCounter = 0;
	int msByFrameRate = MILLISECONDTOSECOND / FRAMERATE;

	while (gameState.engine->IsRunning())
	{
		lastFrameCounter = currentFrameCounter;
		currentFrameCounter =  SDL_GetPerformanceCounter();

		gameState.deltaTime = (float)(currentFrameCounter - lastFrameCounter) / (float)SDL_GetPerformanceFrequency();

		timerFPS = SDL_GetTicks();

		gameState.engine->Event();
		gameState.engine->Update(gameState);
		HandleCollisions();

		timerFPS = SDL_GetTicks() - timerFPS;

		if (timerFPS < msByFrameRate)
		{
			SDL_Delay((msByFrameRate) - timerFPS);
		}

		gameState.engine->Render(gameState);
	}
}
