#ifndef INCLUDED_PLAYER_SKILL_OBJECT_HEADER
#define INCLUDED_PLAYER_SKILL_OBJECT_HEADER
#include"Map.h"
#include"Camera.h"
#include"Player.h"
#include<XEngine.h>
class SkillObject
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime, const X::Math::Vector2& target);
	void Render(const Camera& camera);
	void SetActive(bool active) { mActive = active; }
	bool IsActive() const { return mActive; }
	void Kill();
	X::Math::Circle GetCircleBoundary() const
	{
		return{ mPosition, X::GetSpriteWidth(mTextureId) * 0.6f };
	}

private:
	X::Math::Vector2 mPosition{ 0.0f, 0.0f };
	X::TextureId mTextureId{ 0 };
	bool mActive{ false };
};

#endif // !INCLUDED_PLAYER_SKILL_OBJECT_HEADER
