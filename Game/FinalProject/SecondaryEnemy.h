#ifndef INCLUDED_SECONDARY_ENEMY_HEADER
#define INCLUDED_SECONDARY_ENEMY_HEADER
#include <XEngine.h>
#include <string>
#include "Map.h"
#include "Camera.h"
#include "SecondaryEnemyBulletManager.h"
using std::string;

class SecondaryEnemy
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime, const Map& map, const X::Math::Vector2& target);
	void Render(const Camera& camera);
	bool IsActive() const { return mActive; }
	void Spawn(const X::Math::Vector2& position, const X::Math::Vector2& velocity);
	void Kill() { mActive = false; }
	bool IsStart() const { return mShootReady; }
	bool IsEnd() const { return mShootStop; }
private:
	X::Math::Vector2 mPosition{ 0.0f,0.0f };
	X::Math::Vector2 mVelocity{ 100.0f,0.0f };
	X::TextureId mTextureId{ 0 };
	bool mActive{ false };
	float mStartStootTimer{ 7.0f };
	float mEndShootTimer{ 8.0f };
	bool mShootReady{ false };
	bool mShootStop{ true };
};

#endif // !INCLUDED_SECONDARY_ENEMY_HEADER
