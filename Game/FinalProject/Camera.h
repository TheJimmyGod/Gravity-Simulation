#ifndef INCLUDED_CAMERA_H
#define INCLUDED_CAMERA_H
#include <XEngine.h>
class Camera
{
public:
	X::Math::Vector2 ConvertToScreenPosition(const X::Math::Vector2& worldPosition) const;
	X::Math::Vector2 ConvertToWorldPosition(const X::Math::Vector2& screenPosition) const;

	void SetViewPosition(const X::Math::Vector2& viewPosition)
	{
		mViewPosition = viewPosition;
	}

	void Draw(X::TextureId textureId, const X::Math::Vector2& worldPosition) const;
	void Write(const char* score, const X::Math::Vector2& worldPosition, float size, X::Math::Vector4 color) const;
private:
	X::Math::Vector2 mViewPosition;
};

#endif // !INCLUDED_CAMERA_H
