#include "SecondaryEnemyBulletManager.h"

namespace
{
	std::unique_ptr<SecondaryEnemyBulletManager> sInstance;
}


void SecondaryEnemyBulletManager::StaticInitialize()
{
	sInstance = std::make_unique<SecondaryEnemyBulletManager>();
	sInstance->Initialize();
}

void SecondaryEnemyBulletManager::StaticTerminate()
{
	if (sInstance)
	{
		sInstance->Terminate();
		sInstance.reset();
	}
}

SecondaryEnemyBulletManager * SecondaryEnemyBulletManager::Get()
{
	return sInstance.get();
}

void SecondaryEnemyBulletManager::Initialize()
{
	for (auto& bullet : mBullets)
	{
		bullet.Load();
	}
}

void SecondaryEnemyBulletManager::Terminate()
{
	for (auto& bullet : mBullets)
	{
		bullet.Unload();
	}
}

void SecondaryEnemyBulletManager::Update(float deltaTime, const Map & map)
{
	for (auto& bullet : mBullets)
	{
		bullet.Update(deltaTime, map);
	}
}

void SecondaryEnemyBulletManager::Render(const Camera & camera)
{
	for (auto& bullet : mBullets)
	{
		bullet.Render(camera);
	}
}

void SecondaryEnemyBulletManager::Fire(const X::Math::Vector2 & position, const X::Math::Vector2 & velocity)
{
	mBullets[mNextBullet].Fire(position, velocity);
	mNextBullet = (mNextBullet + 1) % std::size(mBullets);
}
