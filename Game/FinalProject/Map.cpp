#include "Map.h"

void Map::LoadMap(const char * mapFileName)
{
	FILE* file = nullptr;
	fopen_s(&file, mapFileName, "r");
	if (file == nullptr)
		return;
	fscanf_s(file, "Rows: %d\n", &mRows);
	fscanf_s(file, "Columns: %d\n", &mColumns);

	mTileMap = std::make_unique<int[]>(mRows * mColumns);


	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int index = x + (y *mColumns);
			mTileMap[index] = fgetc(file) - '0'; // c = char
		}
		fgetc(file);
	}
	fclose(file);
}

void Map::UnloadMap()
{
	mTileMap.reset();
}

void Map::LoadTextures(const char * textureListFileName)
{
	FILE* file = nullptr;
	fopen_s(&file, textureListFileName, "r");
	if (file == nullptr)
		return;
	fscanf_s(file, "TextureCount: %d\n", &mTextureCount);
	mTextureIds = std::make_unique<X::TextureId[]>(mTextureCount);

	for (int i = 0; i < mTextureCount; ++i)
	{
		char textureName[1024];
		fscanf_s(file, "%s\n", textureName, std::size(textureName));
		mTextureIds[i] = X::LoadTexture(textureName);
	}
	fclose(file);
}

void Map::UnloadTextures()
{
	mTextureIds.reset();
}

void Map::Update(float deltaTime, const Camera & camera)
{

	const char* levelNames[] =
	{
		"Layer01.txt",
		"Layer02.txt",
		"Layer03.txt"
	};
	if (mTimer < X::GetTime())
	{
		UnloadMap();
		LoadMap(levelNames[nextLevelIndex]);
		nextLevelIndex++;
		mTimer += 10.0f;
	}
	if (nextLevelIndex > 2)
	{
		nextLevelIndex = 0;
	}
}

void Map::Render(const Camera & camera)
{
	float tileWidth = static_cast<float>(X::GetSpriteWidth(mTextureIds[0]));
	float tileHeight = static_cast<float>(X::GetSpriteHeight(mTextureIds[0]));

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			index = x + (y *mColumns);
			tile = std::min(mTileMap[index], mTextureCount - 1);
			float posX = (x + 0.5f) * tileWidth;
			float posY = (y + 0.5f) * tileHeight;
			camera.Draw(mTextureIds[tile], { posX, posY });
		}
	}
}

bool Map::CheckCollision(const X::Math::LineSegment & lineSegment) const
{
	float tileWidth = static_cast<float>(X::GetSpriteWidth(mTextureIds[8]));
	float tileHeight = static_cast<float>(X::GetSpriteHeight(mTextureIds[8]));

	int startX = (int)(lineSegment.from.x / tileWidth);
	int startY = (int)(lineSegment.from.y / tileHeight);

	int endX = (int)(lineSegment.to.x / tileWidth);
	int endY = (int)(lineSegment.to.y / tileHeight);

	if (startX < 0 ||
		startX >= mColumns ||
		endX < 0 ||
		endX >= mColumns ||
		startY < 0 ||
		startY >= mRows ||
		endY < 0 ||
		endY >= mRows)
	{
		return false;
	}
	for (int y = startY; y <= endY; y++)
	{
		for (int x = startX; x <= endX; x++)
		{
			int index = x + (y * mColumns);
			int tile = mTileMap[index];
			if (tile == 3 || tile == 4 || tile == 5 || tile == 6 || tile == 7 || tile == 8) // not always true
			{
				return true;
			}
		}
	}
	return false;
}

bool Map::CheckSpecificTileCollision(const X::Math::LineSegment & lineSegment) const
{
	float tileWidth = static_cast<float>(X::GetSpriteWidth(mTextureIds[8]));
	float tileHeight = static_cast<float>(X::GetSpriteHeight(mTextureIds[8]));

	int startX = (int)(lineSegment.from.x / tileWidth);
	int startY = (int)(lineSegment.from.y / tileHeight);

	int endX = (int)(lineSegment.to.x / tileWidth);
	int endY = (int)(lineSegment.to.y / tileHeight);

	if (startX < 0 ||
		startX >= mColumns ||
		endX < 0 ||
		endX >= mColumns ||
		startY < 0 ||
		startY >= mRows ||
		endY < 0 ||
		endY >= mRows)
	{
		return false;
	}
	for (int y = startY; y <= endY; y++)
	{
		for (int x = startX; x <= endX; x++)
		{
			int index = x + (y * mColumns);
			int tile = mTileMap[index];
			if (tile == 1) // not always true
			{
				return true;
			}
		}
	}
	return false;
}

bool Map::CheckSpecificTileCollisionTwo(const X::Math::LineSegment & lineSegment) const
{
	float tileWidth = static_cast<float>(X::GetSpriteWidth(mTextureIds[8]));
	float tileHeight = static_cast<float>(X::GetSpriteHeight(mTextureIds[8]));

	int startX = (int)(lineSegment.from.x / tileWidth);
	int startY = (int)(lineSegment.from.y / tileHeight);

	int endX = (int)(lineSegment.to.x / tileWidth);
	int endY = (int)(lineSegment.to.y / tileHeight);

	if (startX < 0 ||
		startX >= mColumns ||
		endX < 0 ||
		endX >= mColumns ||
		startY < 0 ||
		startY >= mRows ||
		endY < 0 ||
		endY >= mRows)
	{
		return false;
	}
	for (int y = startY; y <= endY; y++)
	{
		for (int x = startX; x <= endX; x++)
		{
			int index = x + (y * mColumns);
			int tile = mTileMap[index];
			if (tile == 2) // not always true
			{
				return true;
			}
		}
	}
	return false;
}
