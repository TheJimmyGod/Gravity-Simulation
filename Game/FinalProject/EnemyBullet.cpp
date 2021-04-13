#include "EnemyBullet.h"

void EnemyBullets::Load(std::string textureName)
{
	mTextureId = X::LoadTexture(textureName.c_str());
}

void EnemyBullets::Unload()
{
	mTextureId = 0;
}

void EnemyBullets::Update(float deltaTime, const Map& map)
{
	if (IsActive())
	{
		float displacementX = 0.0f;
		float displacementY = 0.0f;

		displacementX = mVelocity.x * deltaTime;
		displacementY = mVelocity.y * deltaTime;

		auto boundary = GetBoundary();
		if (displacementX >= 0.0f)
		{
			X::Math::LineSegment rightEage;
			rightEage.from = { boundary.right + displacementX, boundary.top };
			rightEage.to = { boundary.right + displacementX,boundary.bottom };
			//if (map.CheckCollision(rightEage))
			//{
			//	displacementX -= 2.0f;
			//	mVelocity.x *= -1.0f;


			//}
		}
		if (displacementX <= 0.0f)
		{
			X::Math::LineSegment leftEage;
			leftEage.from = { boundary.left + displacementX, boundary.top };
			leftEage.to = { boundary.left + displacementX, boundary.bottom };
			//if (map.CheckCollision(leftEage))
			//{
			//	displacementX += 2.0f;
			//	mVelocity.x *= -1.0f;
			//}
		}
		if (displacementY <= 0.0f)
		{
			X::Math::LineSegment TopEage;
			TopEage.from = { boundary.left, boundary.top + displacementY };
			TopEage.to = { boundary.right, boundary.top + displacementY };
			//if (map.CheckCollision(TopEage))
			//{
			//	displacementY += 2.0f;
			//	mVelocity.y *= -1.0f;
			//}
		}
		if (displacementY >= 0.0f)
		{
			X::Math::LineSegment BottomEage;
			BottomEage.from = { boundary.left, boundary.bottom + displacementY };
			BottomEage.to = { boundary.right,boundary.bottom + displacementY };
			//if (map.CheckCollision(BottomEage))
			//{
			//	displacementY -= 2.0f;
			//	mVelocity.y *= -1.0f;
			//}
		}
		mPosition.x += displacementX;
		mPosition.y += displacementY;

		if (mPosition.x > 1000.0f || mPosition.x < -100.0f || mPosition.y < -100.0f || mPosition.y > 1000.0f)
		{
			Kill();
		}
	}
}

void EnemyBullets::Render(const Camera& camera)
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

void EnemyBullets::Fire(const X::Math::Vector2 & position, const X::Math::Vector2 & velocity)
{
	mPosition = position;
	mVelocity = velocity;
	mActive = true;
}

void EnemyBullets::Kill()
{
	mActive = false;
}

X::Math::Rect EnemyBullets::GetBoundary() const
{
	float Width = (float)X::GetSpriteWidth(mTextureId);
	float Height = (float)X::GetSpriteHeight(mTextureId);
	return {
		mPosition.x - (Width *0.25f), // Left
		mPosition.y - (Height*0.9f), // Top
		mPosition.x + (Width*0.25f), // Right
		mPosition.y }; // Bottom
}

X::Math::Circle EnemyBullets::GetCircleBoundary() const
{
	return { mPosition, X::GetSpriteWidth(mTextureId) * 0.3f };
}
