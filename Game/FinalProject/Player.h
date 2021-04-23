#ifndef INCLUDED_PLAYER_HEADER
#define INCLUDED_PLAYER_HEADER
#include<XEngine.h>
#include "Map.h"
#include "Camera.h"
#include "PlayerCondition.h"
#include "Direction.h"
class Player
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime, const Map& map);
	void Render(const Camera& camera);
	void Death();

	void SetHealth(int health) { mHealth = health; }
	int GetHealth() { return mHealth; }
	void SetDamage(int damage) { mHealth -= damage; IsDamage = true; mDamageTimer = 3.0f; }
	void SetAttack(bool attack) { IsAttack = attack; }
	bool IsAttacked() { return IsAttack; }
	bool IsDead() { return mDeath; }
	bool IsDamaged() { return IsDamage; }
	void Clear()
	{
		IsAttack = false; IsDamage = false; mDeath = false; mHealth = 3;
		mPosition = { X::GetScreenWidth()*0.5f, X::GetScreenHeight()*0.5f };
	}

	void SetCondition(Condition condition) { mCondition = condition; }
	Condition GetCondition() const { return mCondition; }
	void SetDirection(Direction direction) { mDirection = direction; }
	Direction GetDirection() const { return mDirection; }

	const X::Math::Vector2& GetPosition() const { return mPosition; }
	X::Math::Rect GetBoundary() const;

	X::Math::Circle GetCircleBoundary() const;
private:
	X::Math::Vector2 mPosition{ 100.0f,320.0f };
	X::TextureId mTextureId[2]{ 0 };
	float mSpeed{ 600.0f };
	float mTimer{ 1.0f };
	float mDamageTimer{ 0.0f };
	bool IsAttack{ false };
	bool IsDamage{ false };
	bool mDeath{ false };

	X::Math::Vector2 mVelocity{ 0.0f, 0.0f };
	float mframe{ 0.0f };
	int index{ 0 };
	int mHealth{ 3 };

	Direction mDirection{ Direction::Down };
	Condition mCondition{ Condition::Left };
};

#endif // !INCLUDED_PLAYER_HEADER
