#include "PlayerShadow.h"

void Shadow::Load()
{
	mTextureId[0] = X::LoadTexture("Left_Shadow.png");
	mTextureId[1] = X::LoadTexture("Right_Shadow.png");
	SetActive(true);
}

void Shadow::Unload()
{
	mTextureId[0] = 0;
	mTextureId[1] = 0;
}

void Shadow::Update(float deltaTime, const X::Math::Vector2& target)
{
	if (IsActive())
	{
		if (X::IsKeyPressed(X::Keys::A))
		{
			SetIndex(1);
		}
		else if (X::IsKeyPressed(X::Keys::D))
		{
			SetIndex(0);
		}
		if (mSpawnTimer < X::GetTime())
		{
			mPosition.x = target.x;
			mPosition.y = target.y;
			mSpawnTimer += 0.2f;
		}

	}
}

void Shadow::Render(const Camera & camera)
{
	if (IsActive())
	{
		float height = (float)X::GetSpriteHeight(mTextureId[index]);
		auto drawPoistion = mPosition + X::Math::Vector2
		{
			0.0f,
			height* -0.3f
		};
		camera.Draw(mTextureId[index], drawPoistion);
	}
}
