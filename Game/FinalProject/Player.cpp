#include "Player.h"

void Player::Load()
{
	mTextureId[0] = X::LoadTexture("Left.png");
	mTextureId[1] = X::LoadTexture("Right.png");
}

void Player::Unload()
{
	mTextureId[0] = 0;
	mTextureId[1] = 0;
}

void Player::Update(float deltaTime, const Map & map)
{
	float displacementX = 0.0f;
	float displacementY = 0.0f;

	const float gravity = 300.0f;

	if (X::IsKeyPressed(X::Keys::NUMPAD1))
	{
		if (mDirection == Direction::Down)
		{
			SetDirection(Direction::Up);
		}
		else if (mDirection == Direction::Up)
		{
			SetDirection(Direction::Down);
		}
	}

	if (mDirection == Direction::Down)
	{
		mVelocity.y += gravity * deltaTime;
		displacementY = mVelocity.y * deltaTime;
	}

	if (mDirection == Direction::Up)
	{
		mVelocity.y -= gravity * deltaTime;
		displacementY = mVelocity.y * deltaTime;
	}

	if (X::IsKeyDown(X::Keys::D))
	{
		displacementX = mSpeed * 0.5f * deltaTime;
		displacementX += cosf(displacementY / displacementX) * deltaTime;
		SetCondition(Condition::Left);
		index = 0;
		if (mDirection == Direction::Down)
		{
			displacementY -= tanf(displacementX / displacementY) * deltaTime;
		}
		else if (mDirection == Direction::Up)
		{
			displacementY += tanf(displacementX / displacementY) * deltaTime;
		}
	}
	if (X::IsKeyDown(X::Keys::A))
	{
		displacementX = -mSpeed * 0.5f * deltaTime;
		displacementX += cosf(displacementY / displacementX) * deltaTime;
		SetCondition(Condition::Right);
		index = 1;
		if (mDirection == Direction::Down)
		{
			displacementY -= tanf(displacementX / displacementY) * deltaTime;
		}
		else if (mDirection == Direction::Up)
		{
			displacementY += tanf(displacementX / displacementY) * deltaTime;
		}
	}

	if (mDamageTimer > 0.0f)
	{
		mDamageTimer -= deltaTime;
	}
	else
		IsDamage = false;
	
	// Check collision and adjust displacement if needed
	auto boundary = GetBoundary();
	if (displacementX > 0.0f)
	{
		X::Math::LineSegment rightEage;
		rightEage.from = { boundary.right + displacementX, boundary.top };
		rightEage.to = { boundary.right + displacementX,boundary.bottom };
		if (map.CheckCollision(rightEage))
		{
			displacementX = 0.0f;
		}
		if (map.CheckSpecificTileCollision(rightEage))
		{
			mSpeed -= 65.0f;
			displacementY *= 0.5f;
		}
		else if (map.CheckSpecificTileCollisionTwo(rightEage))
		{
			mSpeed = 1000.0f;
			displacementY *= 1.25f;
		}
		else
		{
			mSpeed = 600.0f;
		}
	}
	if (displacementX < 0.0f)
	{
		X::Math::LineSegment leftEage;
		leftEage.from = { boundary.left + displacementX, boundary.top };
		leftEage.to = { boundary.left + displacementX, boundary.bottom };
		if (map.CheckCollision(leftEage))
		{
			displacementX = 0.0f;
		}
		if (map.CheckSpecificTileCollision(leftEage))
		{
			mSpeed = 65.0f;
			displacementY *= 0.5f;
		}
		else if (map.CheckSpecificTileCollisionTwo(leftEage))
		{
			mSpeed = 1000.0f;
			displacementY *= 1.25f;
		}
		else
		{
			mSpeed = 600.0f;
		}
	}
	if (displacementY < 0.0f)
	{
		X::Math::LineSegment TopEage;
		TopEage.from = { boundary.left, boundary.top + displacementY };
		TopEage.to = { boundary.right, boundary.top + displacementY };
		if (map.CheckCollision(TopEage))
		{
			displacementY = 0.0f;
			mVelocity.y = 600.0f;
		}
		if (map.CheckSpecificTileCollision(TopEage))
		{
			mSpeed = 65.0f;
			displacementY *= 0.5f;
		}
		else if (map.CheckSpecificTileCollisionTwo(TopEage))
		{
			mSpeed = 1000.0f;
			displacementY *= 1.5f;
		}
		else
		{
			mSpeed = 600.0f;
		}
	}
	if (displacementY > 0.0f)
	{
		X::Math::LineSegment BottomEage;
		BottomEage.from = { boundary.left, boundary.bottom + displacementY };
		BottomEage.to = { boundary.right,boundary.bottom + displacementY };
		if (map.CheckCollision(BottomEage))
		{
			displacementY = 0.0f;
			mVelocity.y = -600.0f;
		}
		if (map.CheckSpecificTileCollision(BottomEage))
		{
			mSpeed = 65.0f;
			displacementY *= 0.5f;
		}
		else if (map.CheckSpecificTileCollisionTwo(BottomEage))
		{
			mSpeed = 1000.0f;
			displacementY *= 1.5f;
		}
		else
		{
			mSpeed = 600.0f;
		}

	}

	// Update position
	mPosition.x += displacementX;
	mPosition.y += displacementY;

	if ((mPosition.x > X::GetScreenWidth() * 1.25f) || (mPosition.x < 0) || (mPosition.y < 0) || (mPosition.y > X::GetScreenHeight() * 1.25f))
	{
		mPosition.x = 100.0f;
		mPosition.y = 100.0f;
	}
}

void Player::Render(const Camera & camera)
{
	float height = (float)X::GetSpriteHeight(mTextureId[index]);
	auto drawPoistion = mPosition + X::Math::Vector2
	{
		0.0f,
		height* -0.5f
	};
	camera.Draw(mTextureId[index], drawPoistion);
}

void Player::Death()
{
	mDeath = true;
}

X::Math::Rect Player::GetBoundary() const
{
	float Width = (float)X::GetSpriteWidth(mTextureId[index]);
	float Height = (float)X::GetSpriteHeight(mTextureId[index]);
	return {
		mPosition.x - (Width *0.25f), // Left
		mPosition.y - (Height*0.9f), // Top
		mPosition.x + (Width*0.25f), // Right
		mPosition.y }; // Bottom
}

X::Math::Circle Player::GetCircleBoundary() const
{
	return { mPosition, X::GetSpriteWidth(mTextureId[index])*0.5f };
}
