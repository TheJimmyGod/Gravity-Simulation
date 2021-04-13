#include "EnemyManager.h"

namespace
{
	std::unique_ptr<EnemyManager> sInstance;
}

void EnemyManager::StaticInitialize()
{
	sInstance = std::make_unique<EnemyManager>();
	sInstance->Initialize();
}

void EnemyManager::StaticTerminate()
{
	if (sInstance)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

EnemyManager * EnemyManager::Get()
{
	return sInstance.get();
}

void EnemyManager::Initialize()
{
	for (auto& enemy : mEnemy)
	{
		enemy.Load("Enemy.png");
	}
}

void EnemyManager::Terminate()
{
	for (auto& enemy : mEnemy)
	{
		enemy.Unload();
	}
}

void EnemyManager::Update(float deltaTime, const Map& map)
{
	for (auto& enemy : mEnemy)
	{
		enemy.Update(deltaTime, map);
	}
}

void EnemyManager::Render(const Camera& camera)
{
	for (auto& enemy : mEnemy)
	{
		enemy.Render(camera);
	}
}

void EnemyManager::Spawn(const X::Math::Vector2 & position, const X::Math::Vector2 & velocity)
{
	if (!mEnemy[mNextEnemy].IsActive())
	{
		mEnemy[mNextEnemy].Spawn(position, velocity);
	}
	mNextEnemy = (mNextEnemy + 1) % std::size(mEnemy);
}
