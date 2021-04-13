#ifndef INCLUDED_ENEMY_HEADER
#define INCLUDED_ENEMY_HEADER
#include <XEngine.h>
#include <string>
#include "Map.h"
#include "Camera.h"
#include "EnemyBulletManager.h"
using std::string;

class Enemy
{
public:
	void Load(string textureName);
	void Unload();
	void Update(float deltaTime, const Map& map);
	void Render(const Camera& camera);
	void Spawn(const X::Math::Vector2& position, const X::Math::Vector2& velocity);
	void SetVelocity(const X::Math::Vector2& velocity) { mVelocity = velocity; }
	void Kill();
	bool IsActive() const { return mActive; }
	float GetSpeed() { return mSpeed; }
	void SetSpeed(float s) { mSpeed = s; }
	X::Math::Rect GetBoundary() const;
	X::Math::Circle GetCircleBoundary() const;
	const X::Math::Vector2& GetVelocity() const { return mVelocity; }
private:
	X::Math::Vector2 mPosition{ 100.0f,100.0f };
	X::Math::Vector2 mVelocity{ 0.0f,0.0f };
	X::TextureId mTextureId{ 0 };
	float mSpeed{ 250.0f };
	bool mActive{ false };
	float mTimer{ 0.0f };
};

#endif // !INCLUDED_ENEMY_HEADER
