#ifndef ENEMY_BULLET_MANAGER_HEADER
#define ENEMY_BULLET_MANAGER_HEADER

#include"EnemyBullet.h"
#include"Map.h"
#include"Camera.h"
#include<XEngine.h>
class EnemyBulletsManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static EnemyBulletsManager* Get();

public:
	void Initialize();
	void Terminate();

	void Update(float deltaTime, const Map& map);
	void Render(const Camera& camera);

	void Fire(const X::Math::Vector2& position, const X::Math::Vector2& velocity);

	EnemyBullets& GetBullet(size_t index) { return mBullets[index]; }
	size_t GetBulletCount() const { return std::size(mBullets); }

private:
	EnemyBullets mBullets[5];
	int mNextBullet = 0;
};

#endif // !ENEMY_BULLET_MANAGER_HEADER
