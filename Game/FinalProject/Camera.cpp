#include "Camera.h"

X::Math::Vector2 Camera::ConvertToScreenPosition(const X::Math::Vector2 & worldPosition) const
{
	float ScreenWidth = (float)X::GetScreenWidth()*0.5f - mViewPosition.x;
	float ScreenHeight = (float)X::GetScreenHeight()*0.5f - mViewPosition.y;
	return X::Math::Vector2(ScreenWidth + worldPosition.x, ScreenHeight + worldPosition.y);
}

X::Math::Vector2 Camera::ConvertToWorldPosition(const X::Math::Vector2 & screenPosition) const
{
	float worldWidth = screenPosition.x + (mViewPosition.x - (float)X::GetScreenWidth()*0.5f);
	float worldHeight = screenPosition.y + (mViewPosition.y - (float)X::GetScreenHeight()*0.5f);
	return X::Math::Vector2(worldWidth, worldHeight);
}

void Camera::Draw(X::TextureId textureId, const X::Math::Vector2 & worldPosition) const
{
	auto screenPosition = ConvertToScreenPosition(worldPosition);
	if ((screenPosition.y > 0.0f)
		|| (screenPosition.x > 0.0f)
		|| (screenPosition.x > worldPosition.x)
		|| (screenPosition.y > worldPosition.y))
	{
		X::DrawSprite(textureId, screenPosition);
	}
	else
	{
		return;
	}
}

void Camera::Write(const char * score, const X::Math::Vector2 & worldPosition) const
{
	auto screenPosition = ConvertToScreenPosition(worldPosition);
	if ((screenPosition.y > 0.0f)
		|| (screenPosition.x > 0.0f)
		|| (screenPosition.x > worldPosition.x)
		|| (screenPosition.y > worldPosition.y))
	{
		X::DrawScreenText(score, screenPosition.x, screenPosition.y, 30.0f, X::Math::Vector4::LightGreen());
	}
	else
	{
		return;
	}
}
