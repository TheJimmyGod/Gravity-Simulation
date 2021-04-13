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
bool mNewEnemyAppear{ false };

void BackGroundRender();
void CheckCollision();
void CheckBarrierCollision();
void CheckEnemyProjectile();
void CheckSecondaryEnemyProjectile();
bool CollisionCalculation(X::Math::Circle & left, X::Math::Circle & right);

void GameInit()
{
	SpriteAnimations::Info PlayerHitInfo;
	SpriteAnimations::Info EnemyHitInfo;
	SpriteAnimations::Info EngineInfo;
	myMap.LoadMap("Layer01.txt");
	myMap.LoadTextures("Texture01.txt");
	mPlayer.Load();
	mPlayer.SetPosition({ X::GetScreenWidth()*0.5f,X::GetScreenHeight()*0.5f });
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
	const char* mScores = s.c_str();
	mHud.Start(mStart);
	if (X::IsKeyDown(X::Keys::SPACE))
	{
		mStart = true;
	}
	if (mStart)
	{
		if (!mPlayer.IsDead())
		{
			EnemyManager::Get()->Spawn(X::Math::Vector2{ 100.0f, 100.0f }, X::Math::Vector2{ 200.0f, 200.0f });
			EnemyManager::Get()->Spawn(X::Math::Vector2{ 400.0f, 100.0f }, X::Math::Vector2{ 200.0f, 200.0f });

			if (X::IsKeyPressed(X::Keys::NUMPAD2) && !mPlayer.IsAttacked())
			{
				mPlayer.SetAttack(true);
				mSkill.SetActive(true);
			}
			if (mPlayer.IsAttacked())
			{
				if (mAttackTimer < X::GetTime())
				{
					mPlayer.SetAttack(false);
					mSkill.SetActive(false);
					mAttackTimer += 5.0f;
				}
			}

			if (mScoreTimer < X::GetTime())
			{
				mScore += 1;
				mScoreTimer += 1.0f;
			}

			if (mScore > 1000 && !mNewEnemyAppear)
			{
				mSecondEnemy.Spawn(X::Math::Vector2{ 500.0f, 0.0f }, X::Math::Vector2{ 100.0f, 0.0f });
				mNewEnemyAppear = true;
			}

			myMap.Update(deltaTime, myCamera);
			myMap.Render(myCamera);
			mShadow.Update(deltaTime, mPlayer.GetPosition());
			mShadow.Render(myCamera);
			mPlayer.Update(deltaTime, myMap);
			mPlayer.Render(myCamera);
			myCamera.SetViewPosition(mPlayer.GetPosition());
			myCamera.Write(mScores, X::Math::Vector2{ mPlayer.GetPosition().x - 565.0f, mPlayer.GetPosition().y - 350.0f });
			PlayerHit.Update(deltaTime);
			PlayerHit.Render(myCamera);
			EnemyHit.Update(deltaTime);
			EnemyHit.Render(myCamera);
			Engine.Update(deltaTime);
			Engine.Render(myCamera);
			mSecondEnemy.Update(deltaTime, myMap, mPlayer.GetPosition());
			mSecondEnemy.Render(myCamera);
			CheckCollision();
			CheckBarrierCollision();
			CheckEnemyProjectile();
			CheckSecondaryEnemyProjectile();
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

			if (X::IsKeyPressed(X::Keys::NUMPAD1))
			{
				Engine.SetPosition(mPlayer.GetPosition());
				Engine.Play();
			}
		}
		else
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
			mHud.GameOver();
			if (X::IsKeyPressed(X::Keys::SPACE))
			{
				myMap.UnloadMap();
				myMap.LoadMap("Layer01.txt");
				myMap.SetMapIndex(0);
				EnemyManager::Get()->Spawn(X::Math::Vector2{ 100.0f, 100.0f }, X::Math::Vector2{ 200.0f, 200.0f });
				EnemyManager::Get()->Spawn(X::Math::Vector2{ 400.0f, 100.0f }, X::Math::Vector2{ 200.0f, 200.0f });
				mPlayer.SetDead(false);
				mNewEnemyAppear = false;
				mPlayer.SetPosition({ X::GetScreenWidth()*0.5f, X::GetScreenHeight()*0.5f });
				mPlayer.SetHealth(3);
				mScore = 0;
			}
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
	for (size_t i = 0; i < EnemyManager::Get()->GetEnemyCount(); i++)
	{
		auto& mEnemy = EnemyManager::Get()->GetEnemy(i);
		if (!mEnemy.IsActive())
		{
			continue;
		}
		auto mEnemyCircle = mEnemy.GetCircleBoundary();
		auto mPlayerCircle = mPlayer.GetCircleBoundary();
		if (mEnemy.IsActive())
		{
			if (CollisionCalculation(mEnemyCircle, mPlayerCircle))
			{
				if (!mSkill.IsActive() && !mPlayer.IsDamaged())
				{
					mPlayer.SetDamaged(true);
					mEnemy.SetVelocity(X::Math::Vector2{ mEnemy.GetVelocity().x * -1.0f,mEnemy.GetVelocity().y * -1.0f });
					mPlayer.SetDamage(1);
					EnemyHit.SetPosition(mPlayer.GetPosition());
					EnemyHit.Play();
				}
				if (mPlayer.GetHealth() == 0)
				{
					mPlayer.Death();
				}
			}
		}

	}
}

void CheckBarrierCollision()
{
	for (size_t i = 0; i < EnemyManager::Get()->GetEnemyCount(); i++)
	{
		auto& mEnemy = EnemyManager::Get()->GetEnemy(i);
		if (!mEnemy.IsActive())
		{
			continue;
		}
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
					mPlayer.SetDamaged(true);
					PlayerHit.Play();
					mSkill.Kill();
					mScore += 200;
				}
			}
		}

	}
}

void CheckEnemyProjectile()
{
	for (size_t i = 0; i < EnemyBulletsManager::Get()->GetBulletCount(); i++)
	{
		auto& mBullet = EnemyBulletsManager::Get()->GetBullet(i);

		if (!mBullet.IsActive())
		{
			continue;
		}
		auto mBulletCircle = mBullet.GetCircleBoundary();
		auto mPlayerCircle = mPlayer.GetCircleBoundary();
		auto mSkillCircle = mSkill.GetCircleBoundary();
		if (CollisionCalculation(mBulletCircle, mPlayerCircle))
		{
			mBullet.Kill();
			if (!mSkill.IsActive() && !mPlayer.IsDamaged())
			{
				mPlayer.SetDamage(1);
				mPlayer.SetDamaged(true);
				EnemyHit.SetPosition(mPlayer.GetPosition());
				EnemyHit.Play();
			}
			if (mPlayer.GetHealth() == 0)
			{
				mPlayer.Death();
			}
		}
		if (CollisionCalculation(mBulletCircle, mSkillCircle))
		{
			mBullet.Kill();
		}
	}
}

void CheckSecondaryEnemyProjectile()
{
	for (size_t i = 0; i < SecondaryEnemyBulletManager::Get()->GetBulletCount(); i++)
	{
		auto& mBullet = SecondaryEnemyBulletManager::Get()->GetBullet(i);

		if (!mBullet.IsActive())
		{
			continue;
		}
		auto mBulletCircle = mBullet.GetCircleBoundary();
		auto mPlayerCircle = mPlayer.GetCircleBoundary();
		auto mSkillCircle = mSkill.GetCircleBoundary();
		if (CollisionCalculation(mBulletCircle, mPlayerCircle))
		{
			mBullet.Kill();
			if (!mSkill.IsActive() && !mPlayer.IsDamaged())
			{
				mPlayer.SetDamage(1);
				mPlayer.SetDamaged(true);
				EnemyHit.SetPosition(mPlayer.GetPosition());
				EnemyHit.Play();
			}
			if (mPlayer.GetHealth() == 0)
			{
				mPlayer.Death();
			}
		}
		if (CollisionCalculation(mBulletCircle, mSkillCircle))
		{
			mBullet.Kill();
		}
	}
}

bool CollisionCalculation(X::Math::Circle & left, X::Math::Circle & right)
{
	float combinedRadius = left.radius + right.radius;
	float radiusSqr = combinedRadius * combinedRadius;
	X::Math::Vector2 centerToCenter = left.center - right.center;
	float magnitudeSqr = (centerToCenter.x * centerToCenter.x) + (centerToCenter.y * centerToCenter.y);
	return magnitudeSqr < radiusSqr;
}

void BackGroundRender()
{
	X::DrawSprite(bgTextureId, { X::GetScreenWidth()*0.5f, X::GetScreenHeight()*0.5f });
}