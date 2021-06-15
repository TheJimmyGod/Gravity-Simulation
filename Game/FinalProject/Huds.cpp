#include "Huds.h"

void Huds::Update(int health, const X::Math::Vector2& target)
{
	mPosition.x = target.x;
	mPosition.y = target.y;
	switch (health)
	{
	case 3:
		mSprites[0] = X::LoadTexture("Health.png");
		mSprites[1] = X::LoadTexture("Health.png");
		mSprites[2] = X::LoadTexture("Health.png");
		break;
	case 2:
		mSprites[0] = X::LoadTexture("Empty.png");
		mSprites[1] = X::LoadTexture("Health.png");
		mSprites[2] = X::LoadTexture("Health.png");
		break;
	case 1:
		mSprites[1] = X::LoadTexture("Empty.png");
		mSprites[2] = X::LoadTexture("Health.png");
		break;
	case 0:
		mSprites[2] = X::LoadTexture("Empty.png");
		break;
	default:
		break;
	}
}

void Huds::GameOver(int score)
{
	X::DrawScreenText(mText, X::GetScreenWidth()*0.3f, X::GetScreenHeight()*0.4f, 100.0f, X::Math::Vector4::Yellow());
	std::string str = std::to_string(score);
	X::DrawScreenText(str.c_str(), X::GetScreenWidth()*0.8f, X::GetScreenHeight()*0.45f, 50.0f, X::Math::Vector4::Green());
	X::DrawScreenText("Press Space to continue", X::GetScreenWidth()*0.25f, X::GetScreenHeight()*0.6f, 50.0f, X::Math::Vector4::LightGreen());
}

void Huds::load()
{
	mSprites[0] = X::LoadTexture("Health.png");
	mSprites[1] = X::LoadTexture("Health.png");
	mSprites[2] = X::LoadTexture("Health.png");
}

void Huds::Render(const Camera & camera)
{

	float height01 = (float)X::GetSpriteHeight(mSprites[0]);
	float height02 = (float)X::GetSpriteHeight(mSprites[1]);
	float height03 = (float)X::GetSpriteHeight(mSprites[2]);
	auto drawPosition01 = mPosition + X::Math::Vector2
	{
		-30.0f,
		height01* 1.2f
	};
	auto drawPosition02 = mPosition + X::Math::Vector2
	{
		0.0f,
		height02* 1.2f
	};
	auto drawPosition03 = mPosition + X::Math::Vector2
	{
		30.0f,
		height02 * 1.2f
	};
	camera.Draw(mSprites[0], drawPosition01);
	camera.Draw(mSprites[1], drawPosition02);
	camera.Draw(mSprites[2], drawPosition03);
}

void Huds::Unload()
{
	mSprites[0] = 0;
	mSprites[1] = 0;
	mSprites[2] = 0;
}

bool Huds::Start(bool s)
{
	if (!s)
	{
		//X::DrawScreenText("Press Space to play", X::GetScreenWidth()*0.3f, X::GetScreenHeight()*0.4f, 50.0f, X::Math::Vector4::Yellow());
		//X::DrawScreenText("A,D - Move, Num1 - Change Gravity, Num2 - Barrier", X::GetScreenWidth()*0.15f, X::GetScreenHeight()*0.6f, 35.0f, X::Math::Vector4::White());
		return 0;
	}
	else
	{
		return 1;
	}
}
