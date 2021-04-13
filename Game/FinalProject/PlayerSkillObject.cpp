#include "PlayerSkillObject.h"

void SkillObject::Load()
{
	mTextureId = X::LoadTexture("Barrier.png");
}

void SkillObject::Unload()
{
	mTextureId = 0;
}

void SkillObject::Update(float deltaTime, const X::Math::Vector2& target)
{
	if (IsActive())
	{
		mPosition.x = target.x;
		mPosition.y = target.y;
	}
}

void SkillObject::Render(const Camera & camera)
{
	if (IsActive())
	{
		float height = (float)X::GetSpriteHeight(mTextureId);
		auto drawPoistion = mPosition + X::Math::Vector2
		{
			0.0f,
			height* -0.3f
		};
		camera.Draw(mTextureId, drawPoistion);
	}
}

void SkillObject::Kill()
{
	mActive = false;
}
