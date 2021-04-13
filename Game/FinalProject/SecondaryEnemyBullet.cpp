#include "SecondaryEnemyBullet.h"

void SecondaryEnemyBullet::Load()
{
	mTextureId = X::LoadTexture("Lazer.png");
}

void SecondaryEnemyBullet::Unload()
{
	mTextureId = 0;
}

void SecondaryEnemyBullet::Update(float deltaTime, const Map & map)
{
	if (IsActive())
	{
		mPosition.y += mVelocity.y * deltaTime;

		if (mPosition.x > 1000.0f || mPosition.x < -100.0f || mPosition.y < -100.0f || mPosition.y > 1000.0f)
		{
			Kill();
		}
	}
}

void SecondaryEnemyBullet::Render(const Camera & camera)
{
	if (IsActive())
	{
		float height = (float)X::GetSpriteHeight(mTextureId);
		auto drawPosition = mPosition + X::Math::Vector2
		{
			0.0f,
			height*-0.5f
		};
		camera.Draw(mTextureId, drawPosition);
	}
}

void SecondaryEnemyBullet::Fire(const X::Math::Vector2 & position, const X::Math::Vector2 & velocity)
{
	mPosition = position;
	mVelocity = velocity;
	mActive = true;
}

void SecondaryEnemyBullet::Kill()
{
	mActive = false;
}

X::Math::Circle SecondaryEnemyBullet::GetCircleBoundary() const
{
	return { mPosition, X::GetSpriteWidth(mTextureId) * 0.3f };
}
