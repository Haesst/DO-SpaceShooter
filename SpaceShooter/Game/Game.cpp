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

const int FRAMERATE = 120;
const int MILLISECONDTOSECOND = 1000;

void Game::Init(Engine* engine)
{
	m_GameState.m_Engine = engine;
	m_GameState.m_Renderer = engine->GetRenderer();
	m_GameState.m_Window = engine->GetWindow();

	m_AssetManager = engine->GetAssetManager();
	m_EntityManager = &ECS::EntityManager::Get();

	LoadAssets();
	RegisterSystems();
	CreateEntities();
}

void Game::Quit()
{
	delete m_SmallEnemyPool;
	delete m_SmallEnemyBulletPool;
	delete m_PlayerBulletPool;
}

void Game::LoadAssets()
{
	m_AssetManager->Init(m_GameState.m_Engine);
	m_AssetManager->LoadTexture(m_PlayerTextureID, m_PlayerTexturePath);
	m_AssetManager->LoadTexture(m_SmallEnemyTextureID, m_SmallEnemyTexturePath);
	m_AssetManager->LoadTexture(m_EnemyBulletTextureID, m_EnemyBulletTexturePath);
	m_AssetManager->LoadTexture(m_PlayerBulletTextureID, m_PlayerBulletTexturePath);
}

void Game::RegisterSystems()
{
	m_EntityManager->RegisterSystem<ECS::SpriteRenderSystem>();
	m_EntityManager->RegisterSystem<PlayerSystem>();
	m_EntityManager->RegisterSystem<SmallEnemySystem>();
	m_EntityManager->RegisterSystem<BulletSystem>();
	m_EntityManager->RegisterSystem<BulletCollisionSystem>();
	m_EntityManager->RegisterSystem<SmallEnemyWaveSystem>();
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
	CreateBulletPool(300, 1, ECS::EntityTag::PlayerBullet, m_PlayerBulletPool, m_PlayerBulletTextureID, m_PlayerBulletVelocity);
	CreateBulletPool(300, 1, ECS::EntityTag::EnemyBullet, m_SmallEnemyBulletPool, m_EnemyBulletTextureID, m_EnemyBulletVelocity);
}

void Game::CreateBulletPool(int initAmount, int expandBy, ECS::EntityTag tag, EntityPool*& pool, std::string textureID, Vector2D initVelocity)
{
	pool = new EntityPool(initAmount, expandBy);
	SDL_Texture* bulletTexture = m_AssetManager->GetTexture(textureID);
	pool->AddComponentToPool<ECS::SpriteRenderer>(m_GameState.m_Engine->GetRenderer(), bulletTexture);
	int bulletColliderWidth = 0;
	int bulletColliderHeight = 0;
	SDL_QueryTexture(bulletTexture, nullptr, nullptr, &bulletColliderWidth, &bulletColliderHeight);
	pool->AddComponentToPool<BoxCollider2D>(bulletColliderWidth, bulletColliderHeight);
	pool->AddComponentToPool<Bullet>(pool, initVelocity);
	pool->SetPoolEntitiesTag(tag);
}

void Game::CreatePlayer()
{
	ECS::EntityID id = m_EntityManager->AddNewEntity(m_PlayerStartPosition.x, m_PlayerStartPosition.y);
	m_EntityManager->AddComponent<Player>(id);
	SDL_Texture* playerTexture = m_AssetManager->GetTexture(m_PlayerTextureID);
	m_EntityManager->AddComponent<ECS::SpriteRenderer>(id, m_GameState.m_Engine->GetRenderer(), playerTexture);
	int playerColliderWidth = 0;
	int playerColliderHeight = 0;
	SDL_QueryTexture(playerTexture, nullptr, nullptr, &playerColliderWidth, &playerColliderHeight);
	m_EntityManager->AddComponent<BoxCollider2D>(id, playerColliderWidth, playerColliderHeight);
	m_EntityManager->SetEntityActive(id, true);
	m_EntityManager->SetEntityTag(id, ECS::EntityTag::Player);
	m_EntityManager->GetComponent<Player>(id).m_BulletPool = m_PlayerBulletPool;
}

void Game::CreateEnemyPool(int initAmount, int expandBy)
{
	m_SmallEnemyPool = new EntityPool(60, 1);
	SDL_Texture* enemyTexture = m_AssetManager->GetTexture(m_SmallEnemyTextureID);
	m_SmallEnemyPool->AddComponentToPool<ECS::SpriteRenderer>(m_GameState.m_Engine->GetRenderer(), enemyTexture);
	int enemyColliderWidth = 0;
	int enemyColliderHeight = 0;
	SDL_QueryTexture(enemyTexture, nullptr, nullptr, &enemyColliderWidth, &enemyColliderHeight);
	m_SmallEnemyPool->AddComponentToPool<BoxCollider2D>(enemyColliderWidth, enemyColliderHeight);
	m_SmallEnemyPool->AddComponentToPool<SmallEnemy>(m_SmallEnemyBulletPool, m_SmallEnemyPool);
	m_SmallEnemyPool->SetPoolEntitiesTag(ECS::EntityTag::Enemy);
}

void Game::CreateEnemyWaves()
{
	ECS::EntityID enemyWaveID = ECS::EntityManager::Get().AddNewEntity();
	m_EntityManager->AddComponent<SmallEnemyWave>(enemyWaveID, m_SmallEnemyPool);
	SmallEnemyWave* enemyWave = &ECS::EntityManager::Get().GetComponent<SmallEnemyWave>(enemyWaveID);

	enemyWave->m_Waves.push(3);
	enemyWave->m_Waves.push(6);
	enemyWave->m_Waves.push(8);
	enemyWave->m_Waves.push(14);
	enemyWave->m_Waves.push(18);
}

void Game::HandleCollisions()
{
	ECS::System* bulletCollisionSystem = m_EntityManager->GetSystem<BulletCollisionSystem>().get();

	std::set<ECS::EntityID> entities;
	std::vector<ECS::EntityID> killList;

	if (bulletCollisionSystem != nullptr)
	{
		entities = bulletCollisionSystem->GetEntities();
	}


	for (ECS::EntityID entity : entities)
	{
		ECS::EntityTag entityTag = m_EntityManager->GetEntityTag(entity);

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

			ECS::EntityTag otherTag = m_EntityManager->GetEntityTag(otherEntity);

			if ((entityTag == ECS::EntityTag::Player && otherTag == ECS::EntityTag::EnemyBullet) ||
				(entityTag == ECS::EntityTag::Enemy && otherTag == ECS::EntityTag::PlayerBullet))
			{
				BoxCollider2D collider = m_EntityManager->GetComponent<BoxCollider2D>(entity);
				BoxCollider2D otherCollider = m_EntityManager->GetComponent<BoxCollider2D>(otherEntity);

				if (AABB(collider.m_Box, otherCollider.m_Box))
				{
					Collide(entity, otherEntity, &killList);
				}
			}
		}
	}

	for (ECS::EntityID entity : killList)
	{
		m_EntityManager->SetEntityActive(entity, false);
	}
}

bool Game::AABB(const SDL_Rect& rectA, const SDL_Rect& rectB) const
{
	return (rectA.x + rectA.w >= rectB.x && rectB.x + rectB.w >= rectA.x &&
		rectA.y + rectA.h >= rectB.y && rectB.y + rectB.h >= rectA.y);
}

void Game::Collide(const ECS::EntityID entity, const ECS::EntityID otherEntity, std::vector<ECS::EntityID>* killList)
{
	ECS::EntityTag entityTag = m_EntityManager->GetEntityTag(entity);
	ECS::EntityTag otherTag = m_EntityManager->GetEntityTag(otherEntity);

	if (entityTag == ECS::EntityTag::Player)
	{
		if (otherTag == ECS::EntityTag::EnemyBullet)
		{
			killList->push_back(otherEntity);

			Player* player = &m_EntityManager->GetComponent<Player>(entity);
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

	while (m_GameState.m_Engine->IsRunning())
	{
		lastFrameCounter = currentFrameCounter;
		currentFrameCounter =  SDL_GetPerformanceCounter();

		m_GameState.m_DeltaTime = (float)(currentFrameCounter - lastFrameCounter) / (float)SDL_GetPerformanceFrequency();

		timerFPS = SDL_GetTicks();

		m_GameState.m_Engine->Event();
		m_GameState.m_Engine->Update(m_GameState);
		HandleCollisions();

		timerFPS = SDL_GetTicks() - timerFPS;

		if (timerFPS < msByFrameRate)
		{
			SDL_Delay((msByFrameRate) - timerFPS);
		}

		m_GameState.m_Engine->Render(m_GameState);
	}
}
