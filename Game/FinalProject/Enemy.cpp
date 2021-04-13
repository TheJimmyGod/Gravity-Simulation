#include "Enemy.h"

void Enemy::Load(string textureName)
{
	mTextureId = X::LoadTexture(textureName.c_str());
}

void Enemy::Unload()
{
	mTextureId = 0;
}

void Enemy::Update(float deltaTime, const Map& map)
{
	if (IsActive())
	{

		if ((mPosition.x > X::GetScreenWidth() * 1.25f) || (mPosition.x < 0) || (mPosition.y < 0) || (mPosition.y > X::GetScreenHeight() * 1.25f))
		{
			mPosition.x = 100.0f;
			mPosition.y = 100.0f;
		}
		float displacementX = 0.0f;
		float displacementY = 0.0f;

		displacementX = mVelocity.x * deltaTime;
		displacementY = mVelocity.y * deltaTime;
		displacementX += tanf(displacementX / displacementY);
		displacementY += cosf(displacementX / displacementY);
		auto boundary = GetBoundary();
		if (displacementX >= 0.0f)
		{
			X::Math::LineSegment rightEage;
			rightEage.from = { boundary.right + displacementX, boundary.top };
			rightEage.to = { boundary.right + displacementX,boundary.bottom };
			if (map.CheckCollision(rightEage))
			{
				displacementX -= 2.0f;
				mVelocity.x *= -1.0f;
				EnemyBulletsManager::Get()->Fire(mPosition, X::Math::Vector2{ -600.0, 0.0f });
				
			}
		}
		
		if (displacementX <= 0.0f)
		{
			X::Math::LineSegment leftEage;
			leftEage.from = { boundary.left + displacementX, boundary.top };
			leftEage.to = { boundary.left + displacementX, boundary.bottom };
			if (map.CheckCollision(leftEage))
			{
				displacementX += 2.0f;
				mVelocity.x *= -1.0f;
				EnemyBulletsManager::Get()->Fire(mPosition, X::Math::Vector2{ 600.0, 0.0f });
			}
		}
		if (displacementY <= 0.0f)
		{
			X::Math::LineSegment TopEage;
			TopEage.from = { boundary.left, boundary.top + displacementY };
			TopEage.to = { boundary.right, boundary.top + displacementY };
			if (map.CheckCollision(TopEage))
			{
				displacementY += 2.0f;
				mVelocity.y *= -1.0f;
				EnemyBulletsManager::Get()->Fire(mPosition, X::Math::Vector2{ 0.0, 600.0f });
			}
		}
		if (displacementY >= 0.0f)
		{
			X::Math::LineSegment BottomEage;
			BottomEage.from = { boundary.left, boundary.bottom + displacementY };
			BottomEage.to = { boundary.right,boundary.bottom + displacementY };
			if (map.CheckCollision(BottomEage))
			{
				displacementY -= 2.0f;
				mVelocity.y *= -1.0f;
				EnemyBulletsManager::Get()->Fire(mPosition, X::Math::Vector2{ 0.0, -600.0f });
			}
		}


		mPosition.x += displacementX;
		mPosition.y += displacementY;
	}
}

void Enemy::Render(const Camera& camera)
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

void Enemy::Spawn(const X::Math::Vector2 & position, const X::Math::Vector2 & velocity)
{
	mPosition = position;
	mVelocity = velocity;
	mActive = true;
}

void Enemy::Kill()
{
	mActive = false;
}

X::Math::Rect Enemy::GetBoundary() const
{
	float Width = (float)X::GetSpriteWidth(mTextureId);
	float Height = (float)X::GetSpriteHeight(mTextureId);
	return {
		mPosition.x - (Width *0.25f), // Left
		mPosition.y - (Height*0.9f), // Top
		mPosition.x + (Width*0.25f), // Right
		mPosition.y }; // Bottom
}

X::Math::Circle Enemy::GetCircleBoundary() const
{
	return { mPosition, X::GetSpriteWidth(mTextureId) * 0.3f };
}
