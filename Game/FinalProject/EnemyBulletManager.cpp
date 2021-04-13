#include "EnemyBulletManager.h"

namespace
{
	std::unique_ptr<EnemyBulletsManager> sInstance;
}

void EnemyBulletsManager::StaticInitialize()
{
	sInstance = std::make_unique<EnemyBulletsManager>();
	sInstance->Initialize();
}

void EnemyBulletsManager::StaticTerminate()
{
	if (sInstance)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

EnemyBulletsManager * EnemyBulletsManager::Get()
{
	return sInstance.get();
}

void EnemyBulletsManager::Initialize()
{
	for (auto& bullet : mBullets)
	{
		bullet.Load("Enemy_Bullet.png");
	}
}

void EnemyBulletsManager::Terminate()
{
	for (auto& bullet : mBullets)
	{
		bullet.Unload();
	}
}

void EnemyBulletsManager::Update(float deltaTime, const Map& map)
{
	for (auto& bullet : mBullets)
	{
		bullet.Update(deltaTime, map);
	}
}

void EnemyBulletsManager::Render(const Camera& camera)
{
	for (auto& bullet : mBullets)
	{
		bullet.Render(camera);
	}
}

void EnemyBulletsManager::Fire(const X::Math::Vector2 & position, const X::Math::Vector2 & velocity)
{
	mBullets[mNextBullet].Fire(position, velocity);
	mNextBullet = (mNextBullet + 1) % std::size(mBullets);
}
