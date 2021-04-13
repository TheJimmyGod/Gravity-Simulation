#ifndef INCLUDED_PLAYER_SHADOW_HEADER
#define INCLUDED_PLAYER_SHADOW_HEADER
#include <XEngine.h>
#include <string>
#include "Camera.h"
#include "Map.h"
using std::string;

class Shadow
{
public:
	void Load();
	void Unload();
	void Update(float deltaTime, const X::Math::Vector2& target);
	void Render(const Camera& camera);
	void SetActive(bool active) { mActive = active; }
	void Kill() { mActive = false; }
	bool IsActive() const { return mActive; }
	const X::Math::Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const X::Math::Vector2& position) { mPosition = position; }
	void SetIndex(int i) { index = i; }
	int GetIndex() const { return index; }
private:
	bool mActive{ false };
	int index{ 0 };
	float mSpawnTimer{ 0.2f };
	X::Math::Vector2 mPosition{ 0.0f,0.0f };
	X::TextureId mTextureId[2] { 0 };
};

#endif // !INCLUDED_PLAYER_SHADOW_HEADER
