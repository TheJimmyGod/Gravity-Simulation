#include "SecondaryEnemy.h"

void SecondaryEnemy::Load()
{
	mTextureId = X::LoadTexture("carrier_17.png");
}

void SecondaryEnemy::Unload()
{
	mTextureId = 0;
}

void SecondaryEnemy::Update(float deltaTime, const Map & map, const X::Math::Vector2& target)
{
	if (IsActive())
	{
		if (target.x > mPosition.x)
		{
			mPosition.x += mVelocity.x * deltaTime;
		}
		else if (target.x < mPosition.x)
		{
			mPosition.x += -mVelocity.x * deltaTime;
		}

		if (mStartStootTimer < X::GetTime() && IsEnd())
		{
			mShootReady = true;
			mShootStop = false;
			mStartStootTimer += 7.0f;
		}

		if (IsStart())
		{
			mVelocity.x = 0.0f;
			SecondaryEnemyBulletManager::Get()->Fire(mPosition, X::Math::Vector2{ 0.0, 250.0f });
		}

		if (mEndShootTimer < X::GetTime() && IsStart())
		{
			mShootStop = true;
			mShootReady = false;
			mVelocity.x = 100.0f;
			mEndShootTimer += 7.0f;
		}

	}
}

void SecondaryEnemy::Render(const Camera & camera)
{
	if (IsActive())
	{
		float height = (float)X::GetSpriteHeight(mTextureId);
		auto drawPoistion = mPosition + X::Math::Vector2
		{
			0.0f,
			height* -0.5f
		};
		camera.Draw(mTextureId, drawPoistion);
	}
}

void SecondaryEnemy::Spawn(const X::Math::Vector2 & position, const X::Math::Vector2 & velocity)
{
	mPosition = position;
	mVelocity = velocity;
	mActive = true;
}
