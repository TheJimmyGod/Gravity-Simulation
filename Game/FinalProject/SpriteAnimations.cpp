#include "SpriteAnimations.h"

void SpriteAnimations::Load(Info info)
{
	mInfo = std::move(info);
	mSpriteSheetId = X::LoadTexture(mInfo.fileName.c_str());
	mTimer = 0.0f;
}

void SpriteAnimations::Unload()
{
	mSpriteSheetId = 0;
}

void SpriteAnimations::Update(float deltaTime)
{
	if (mActive)
	{
		mTimer += mInfo.framePerSecond * deltaTime;
		if (!mInfo.looping && mTimer >= mInfo.frameCount)
		{
			Stop();
		}
	}
}

void SpriteAnimations::Render(const Camera & camera) const
{
	if (mActive)
	{
		int frameIndex = static_cast<int>(mTimer) % mInfo.frameCount;
		// Repeat increasingly parts e.g) 0 1 2 3 0 1 2 3
		int column = frameIndex % mInfo.columns;
		// Repeat consistantly parts e.g) 0 0 0 0 1 1 1 1
		int row = frameIndex / mInfo.rows;
		float frameWidth = X::GetSpriteWidth(mSpriteSheetId) / mInfo.columns;
		float frameHeight = X::GetSpriteWidth(mSpriteSheetId) / mInfo.rows;
		X::Math::Rect rect;
		rect.left = column * frameWidth;
		rect.right = (column + 1) * frameWidth;
		rect.top = row * frameHeight;
		rect.bottom = (row + 1) * frameHeight;
		X::DrawSprite(mSpriteSheetId, rect, camera.ConvertToScreenPosition(mPosition));
	}
}

void SpriteAnimations::Play()
{
	mTimer = 0.0f;
	mActive = true;
}

void SpriteAnimations::Stop()
{
	mActive = false;
}
