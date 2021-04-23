#ifndef INCLUDED_HUD_HEADER
#define INCLUDED_HUD_HEADER
#include <XEngine.h>
#include "Camera.h"
class Huds
{
public:
	void Update(int health, const X::Math::Vector2& target);
	void GameOver(int score);
	void load();
	void Render(const Camera& camera);
	void Unload();
	bool Start(bool s);

	const X::Math::Vector2& GetPosition() const { return mPosition; }
private:
	const char* mText{ "Game Over" };
	X::TextureId mSprites[3];
	X::Math::Vector2 mPosition{ 500.0f, -300.0f };
	int index{ 3 };
};

#endif // !HUD_HEADER