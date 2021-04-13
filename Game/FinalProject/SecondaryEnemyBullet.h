#ifndef INCLUDED_SECONDARY_ENEMY_BULLET_HEADER
#define INCLUDED_SECONDARY_ENEMY_BULLET_HEADER
#include <XEngine.h>
#include "Camera.h"
#include "Map.h"
class SecondaryEnemyBullet
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime, const Map& map);
	void Render(const Camera& camera);
	void Fire(const X::Math::Vector2& position, const X::Math::Vector2& velocity);
	void Kill();
	bool IsActive() const { return mActive; }
	X::Math::Circle GetCircleBoundary() const;
private:
	X::Math::Vector2 mPosition{ 0.0f,0.0f };
	X::Math::Vector2 mVelocity{ 0.0f,250.0f };
	X::TextureId mTextureId{ 0 };
	bool mActive{ false };
};

#endif // !INCLUDED_SECONDARY_ENEMY_BULLET_HEADER