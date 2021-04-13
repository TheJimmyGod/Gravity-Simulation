#ifndef INCLUDED_MAP_HEADER
#define INCLUDED_MAP_HEADER
#include <XEngine.h>
#include "Camera.h"

class Map
{
public:
	void LoadMap(const char* mapFileName);
	void UnloadMap();

	void LoadTextures(const char* textureListFileName);
	void UnloadTextures();
	void SetMapIndex(int index) { nextLevelIndex = index; }
	void Update(float deltaTime, const Camera& camera);
	void Render(const Camera& camera);

	bool CheckCollision(const X::Math::LineSegment& lineSegment) const;
	bool CheckSpecificTileCollision(const X::Math::LineSegment& lineSegment) const;
	bool CheckSpecificTileCollisionTwo(const X::Math::LineSegment& lineSegment) const;
private:
	int mRows{ 0 };
	int mColumns{ 0 };
	int tile{ 0 };
	int index{ 0 };
	int mTextureCount{ 0 };
	int nextLevelIndex{ 0 };

	float mTimer{ 10.0f };

	std::unique_ptr<int[]> mTileMap;
	std::unique_ptr < X::TextureId[] > mTextureIds;

};

#endif // !INCLUDED_MAP_HEADER
