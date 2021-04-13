#ifndef ENEMY_MANAGER_HEADER
#define ENEMY_MANAGER_HEADER

#include <XEngine.h>
#include "Enemy.h"
#include "Map.h"
#include "Camera.h"

class EnemyManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static EnemyManager* Get();

public:
	void Initialize();
	void Terminate();

	void Update(float deltaTime, const Map& map);
	void Render(const Camera& camera);

	void Spawn(const X::Math::Vector2& position, const X::Math::Vector2& velocity);
	Enemy& GetEnemy(size_t index) { return mEnemy[index]; }
	size_t GetEnemyCount() const { return std::size(mEnemy); }
private:
	Enemy mEnemy[2];
	int mNextEnemy = 0;
};

#endif // !ENEMY_MANAGER_HEADER
