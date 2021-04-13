#ifndef INCLUDED_SPRITE_ANIMATIONS_HEADER
#define INCLUDED_SPRITE_ANIMATIONS_HEADER

#include<XEngine.h>
#include"Camera.h"
class SpriteAnimations
{
public:
	struct Info
	{
		std::string fileName;
		int rows{ 0 };
		int columns{ 0 };
		int frameCount{ 0 };
		float framePerSecond{ 0.0f };
		bool looping{ false };
	};
	void Load(Info info);
	void Unload();
	void Update(float deltaTime);
	void Render(const Camera& camera) const;
	const X::Math::Vector2& GetPosition() { return mPosition; }
	void SetPosition(const X::Math::Vector2& position) { mPosition = position; }

	void Play();
	void Stop();

private:
	X::TextureId mSpriteSheetId{ 0 };
	Info mInfo;
	X::Math::Vector2 mPosition{ 0.0f, 0.0f };
	float mTimer{ 0.0f };
	bool mActive{ false };
};

#endif // !INCLUDED_SPRITE_ANIMATIONS_HEADER
