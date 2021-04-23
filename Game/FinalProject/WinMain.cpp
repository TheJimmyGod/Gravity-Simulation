#include <XEngine.h>
#include "Camera.h"
#include "Map.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "SpriteAnimations.h"
#include "PlayerSkillObject.h"
#include "Huds.h"
#include "PlayerShadow.h"
#include "SecondaryEnemy.h"
#include "SecondaryEnemyBulletManager.h"
#include "Direction.h"

namespace
{
	bool CollisionCalculation(X::Math::Circle & left, X::Math::Circle & right)
	{
		float combinedRadius = left.radius + right.radius;
		float radiusSqr = combinedRadius * combinedRadius;
		X::Math::Vector2 centerToCenter = left.center - right.center;
		float magnitudeSqr = (centerToCenter.x * centerToCenter.x) + (centerToCenter.y * centerToCenter.y);
		return magnitudeSqr < radiusSqr;
	}
}

Map myMap;
Player mPlayer;
SkillObject mSkill;
Camera myCamera;
Shadow mShadow;
SecondaryEnemy mSecondEnemy;
SpriteAnimations PlayerHit;
SpriteAnimations EnemyHit;
SpriteAnimations Engine;
Huds mHud;
int mScore{ 0 };
X::TextureId bgTextureId{0};
float mAttackTimer{ 0.0f };

float mScoreTimer{ 1.0f };
bool mStart{ false };

void BackGroundRender();
void CheckCollision();
void Reset();

void GameInit()
{
	SpriteAnimations::Info PlayerHitInfo;
	SpriteAnimations::Info EnemyHitInfo;
	SpriteAnimations::Info EngineInfo;
	myMap.LoadMap("Layer01.txt");
	myMap.LoadTextures("Texture01.txt");
	mPlayer.Load();
	mPlayer.Clear();
	mHud.load();
	mSkill.Load();
	mShadow.Load();
	mSecondEnemy.Load();
	EnemyManager::StaticInitialize();
	EnemyBulletsManager::StaticInitialize();
	SecondaryEnemyBulletManager::StaticInitialize();
	bgTextureId = X::LoadTexture("BackGround.jpg");
	PlayerHitInfo.fileName = "Spark.png";
	PlayerHitInfo.rows = 5;
	PlayerHitInfo.columns = 5;
	PlayerHitInfo.frameCount = 25;
	PlayerHitInfo.framePerSecond = 25.0f;
	PlayerHitInfo.looping = false;

	EnemyHitInfo.fileName = "blood_effect.png";
	EnemyHitInfo.rows = 4;
	EnemyHitInfo.columns = 4;
	EnemyHitInfo.frameCount = 16;
	EnemyHitInfo.framePerSecond = 16.0f;
	EnemyHitInfo.looping = false;


	EngineInfo.fileName = "EngineFire.png";
	EngineInfo.columns = 4;
	EngineInfo.rows = 4;
	EngineInfo.frameCount = 16;
	EngineInfo.framePerSecond = 16.0f;
	EngineInfo.looping = false;

	PlayerHit.Load(PlayerHitInfo);
	EnemyHit.Load(EnemyHitInfo);
	Engine.Load(EngineInfo);

	

}

bool GameLoop(float deltaTime)
{
	BackGroundRender();
	string s = std::to_string(mScore);
	mHud.Start(mStart);
	if (X::IsKeyDown(X::Keys::SPACE))
		mStart = true;

	if (mStart)
	{
		if (!mPlayer.IsDead())
		{
			EnemyManager::Get()->Spawn(X::Math::Vector2{ 100.0f, 100.0f }, X::Math::Vector2{ 200.0f, 200.0f });
			EnemyManager::Get()->Spawn(X::Math::Vector2{ 400.0f, 100.0f }, X::Math::Vector2{ 200.0f, 200.0f });

			if (X::IsKeyPressed(X::Keys::NUMPAD2))
			{
				if (mPlayer.IsAttacked() == false)
				{
					mPlayer.SetAttack(true);
					mSkill.SetActive(true);
					PlayerHit.SetPosition(mPlayer.GetPosition());
					PlayerHit.Play();
					mAttackTimer = 5.0f;
				}
			}
			if (mPlayer.IsAttacked())
			{
				if (mAttackTimer <= 0.0f)
					mPlayer.SetAttack(false);
				if (mAttackTimer <= 3.0f)
					mSkill.SetActive(false);
			}
			if(mAttackTimer > 0.0f)
				mAttackTimer -= deltaTime;
			if (mScoreTimer < X::GetTime())
			{
				mScore += 1;
				mScoreTimer += 1.0f;
			}

			if (mScore > 1000 && mSecondEnemy.IsActive() == false)
				mSecondEnemy.Spawn(X::Math::Vector2{ 500.0f, 0.0f }, X::Math::Vector2{ 100.0f, 0.0f });

			myMap.Update(deltaTime, myCamera);
			myMap.Render(myCamera);
			mShadow.Update(deltaTime, mPlayer.GetPosition());
			mShadow.Render(myCamera);
			mPlayer.Update(deltaTime, myMap);
			mPlayer.Render(myCamera);
			myCamera.SetViewPosition(mPlayer.GetPosition());
			myCamera.Write(s.c_str(), X::Math::Vector2{ mPlayer.GetPosition().x - 565.0f, mPlayer.GetPosition().y - 350.0f },30.0f, X::Math::Vector4::LightGreen());
			PlayerHit.Update(deltaTime);
			PlayerHit.Render(myCamera);
			EnemyHit.Update(deltaTime);
			EnemyHit.Render(myCamera);
			Engine.Update(deltaTime);
			Engine.Render(myCamera);
			mSecondEnemy.Update(deltaTime, myMap, mPlayer.GetPosition());
			mSecondEnemy.Render(myCamera);
			CheckCollision();

			EnemyManager::Get()->Update(deltaTime, myMap);
			EnemyManager::Get()->Render(myCamera);
			EnemyBulletsManager::Get()->Update(deltaTime, myMap);
			EnemyBulletsManager::Get()->Render(myCamera);
			SecondaryEnemyBulletManager::Get()->Update(deltaTime, myMap);
			SecondaryEnemyBulletManager::Get()->Render(myCamera);

			mHud.Update(mPlayer.GetHealth(), mPlayer.GetPosition());
			mHud.Render(myCamera);

			mSkill.Update(deltaTime, mPlayer.GetPosition());
			mSkill.Render(myCamera);
			if (static_cast<int>(mAttackTimer) != 0)
			{
				std::string str = std::to_string(static_cast<int>(mAttackTimer));
				str = "CoolTime: " + str;
				myCamera.Write(str.c_str(), X::Math::Vector2(mPlayer.GetPosition().x + 30.0f, mPlayer.GetPosition().y - 50.0f), 15.0f, X::Math::Vector4::White());
			}

			switch (mPlayer.GetDirection())
			{
			case Direction::Down:
				myCamera.Write("Down", X::Math::Vector2(mPlayer.GetPosition().x + 30.0f, mPlayer.GetPosition().y - 20.0f), 15.0f, X::Math::Vector4::White());
				break;
			case Direction::Up:
				myCamera.Write("Up", X::Math::Vector2(mPlayer.GetPosition().x + 30.0f, mPlayer.GetPosition().y - 20.0f), 15.0f, X::Math::Vector4::White());
				break;
			default:
				break;
			}

			if (X::IsKeyPressed(X::Keys::NUMPAD1))
			{
				Engine.SetPosition(mPlayer.GetPosition());
				Engine.Play();
			}
		}
		else
		{
			mHud.GameOver(mScore);
			if (X::IsKeyPressed(X::Keys::SPACE))
				Reset();
		}
	}
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

void GameCleanup()
{
	// delete is just to delete a byte, but delete[] is able to delete a bunch of memory.
	EnemyManager::StaticTerminate();
	EnemyBulletsManager::StaticTerminate();
	myMap.UnloadMap();
	myMap.UnloadTextures();
	mPlayer.Unload();
	PlayerHit.Unload();
	mSkill.Unload();
	EnemyHit.Unload();
	Engine.Unload();
	mHud.Unload();
	mShadow.Unload();
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	GameInit();
	X::Run(GameLoop);
	GameCleanup();
	X::Stop();
	return 0;
}

void CheckCollision()
{
	auto mPlayerCircle = mPlayer.GetCircleBoundary();
	auto mSkillCircle = mSkill.GetCircleBoundary();
	for (size_t i = 0; i < EnemyManager::Get()->GetEnemyCount(); i++)
	{
		auto& mEnemy = EnemyManager::Get()->GetEnemy(i);
		if (!mEnemy.IsActive())
			continue;
		auto mEnemyCircle = mEnemy.GetCircleBoundary();
		auto mBarrierCircle = mSkill.GetCircleBoundary();
		if (mEnemy.IsActive())
		{
			if (CollisionCalculation(mEnemyCircle, mBarrierCircle))
			{
				if (mSkill.IsActive())
				{
					mEnemy.SetVelocity(X::Math::Vector2{ mEnemy.GetVelocity().x * -1.0f,mEnemy.GetVelocity().y * -1.0f });
					PlayerHit.SetPosition(mPlayer.GetPosition());
					PlayerHit.Play();
					mPlayer.SetDamage(0);
					mSkill.Kill();
					mScore += 200;
				}
			}
		}
	}

	if (mSkill.IsActive())
		return;

	for (size_t i = 0; i < EnemyManager::Get()->GetEnemyCount(); i++)
	{
		auto& mEnemy = EnemyManager::Get()->GetEnemy(i);
		auto mEnemyCircle = mEnemy.GetCircleBoundary();
		
		if (mEnemy.IsActive())
		{
			if (CollisionCalculation(mEnemyCircle, mPlayerCircle))
			{
				if (mPlayer.IsDamaged())
					return;
				mEnemy.SetVelocity(X::Math::Vector2{ mEnemy.GetVelocity().x * -1.0f,mEnemy.GetVelocity().y * -1.0f });
				mPlayer.SetDamage(1);
				EnemyHit.SetPosition(mPlayer.GetPosition());
				EnemyHit.Play();
				if (mPlayer.GetHealth() == 0)
					mPlayer.Death();
			}
		}
		else
			continue;

	}
	for (size_t i = 0; i < EnemyBulletsManager::Get()->GetBulletCount(); i++)
	{
		auto& mBullet = EnemyBulletsManager::Get()->GetBullet(i);

		if (!mBullet.IsActive())
			continue;
		auto mBulletCircle = mBullet.GetCircleBoundary();
		if (CollisionCalculation(mBulletCircle, mPlayerCircle))
		{
			mBullet.Kill();

			if (mPlayer.IsDamaged())
				return;
			mPlayer.SetDamage(1);
			EnemyHit.SetPosition(mPlayer.GetPosition());
			EnemyHit.Play();
			if (mPlayer.GetHealth() == 0)
				mPlayer.Death();

		}
		else if (CollisionCalculation(mBulletCircle, mSkillCircle))
			mBullet.Kill();
	}

	for (size_t i = 0; i < SecondaryEnemyBulletManager::Get()->GetBulletCount(); i++)
	{
		auto& mBullet = SecondaryEnemyBulletManager::Get()->GetBullet(i);

		if (!mBullet.IsActive())
			continue;
		auto mBulletCircle = mBullet.GetCircleBoundary();
		if (CollisionCalculation(mBulletCircle, mPlayerCircle))
		{
			mBullet.Kill();
			if (mPlayer.IsDamaged())
				return;
			mPlayer.SetDamage(1);
			EnemyHit.SetPosition(mPlayer.GetPosition());
			EnemyHit.Play();
			if (mPlayer.GetHealth() == 0)
				mPlayer.Death();

		}
		else if (CollisionCalculation(mBulletCircle, mSkillCircle))
			mBullet.Kill();
	}
}

void BackGroundRender()
{
	X::DrawSprite(bgTextureId, { X::GetScreenWidth()*0.5f, X::GetScreenHeight()*0.5f });
}

void Reset()
{
	for (size_t i = 0; i < EnemyManager::Get()->GetEnemyCount(); i++)
	{
		auto& mEnemy = EnemyManager::Get()->GetEnemy(i);
		mEnemy.Kill();
	}
	for (size_t i = 0; i < EnemyBulletsManager::Get()->GetBulletCount(); i++)
	{
		auto& mBullet = EnemyBulletsManager::Get()->GetBullet(i);
		mBullet.Kill();
	}
	for (size_t i = 0; i < SecondaryEnemyBulletManager::Get()->GetBulletCount(); i++)
	{
		auto& mBullet = SecondaryEnemyBulletManager::Get()->GetBullet(i);
		mBullet.Kill();
	}
	mSecondEnemy.Kill();

	myMap.UnloadMap();
	myMap.LoadMap("Layer01.txt");
	myMap.SetMapIndex(0);
	EnemyManager::Get()->Spawn(X::Math::Vector2{ 100.0f, 100.0f }, X::Math::Vector2{ 200.0f, 200.0f });
	EnemyManager::Get()->Spawn(X::Math::Vector2{ 400.0f, 100.0f }, X::Math::Vector2{ 200.0f, 200.0f });
	mPlayer.Clear();
	mScore = 0;
}