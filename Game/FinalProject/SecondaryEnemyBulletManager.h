#ifndef INCLUDED_SECONDARY_ENEMY_BULLET_MANAGER_HEADER
#define INCLUDED_SECONDARY_ENEMY_BULLET_MANAGER_HEADER
#include"Map.h"
#include"Camera.h"
#include"SecondaryEnemyBullet.h"
#include<XEngine.h>

class SecondaryEnemyBulletManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static SecondaryEnemyBulletManager* Get();

public:
	void Initialize();
	void Terminate();

	void Update(float deltaTime, const Map& map);
	void Render(const Camera& camera);

	void Fire(const X::Math::Vector2& position, const X::Math::Vector2& velocity);

	SecondaryEnemyBullet& GetBullet(size_t index) { return mBullets[index]; }
	size_t GetBulletCount() const { return std::size(mBullets); }

private:
	SecondaryEnemyBullet mBullets[300];
	int mNextBullet = 0;
};
#endif // !INCLUDED_SECONDARY_ENEMY_BULLET_MANAGER_HEADER
