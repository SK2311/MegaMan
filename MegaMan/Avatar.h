#pragma once
#include "SoundEffect.h"

class Level;
class Texture;
class BulletManager;
class EnemyManager;

struct Vector2f;

class Avatar final
{
public:
	enum class AvatarState
	{
		idle,
		moving,
		jumping,
		shootingIdle,
		shootingRunning,
		climbing,
		die,
		shootingJump
	};

	explicit Avatar();
	Avatar(const Avatar& other) = delete;
	Avatar& operator=(const Avatar& other) = delete;
	Avatar(Avatar&& other) = delete;
	Avatar& operator=(Avatar&& other) = delete;
	~Avatar();

	void Update(float elapsedSec, const Level* level, BulletManager* bulletManager, EnemyManager* enemyManager, const bool isMuted);
	void Draw() const;
	Rectf GetShape() const;
	void SetShooting(bool isShooting);
	bool GetShooting() const;
	float GetDirection() const;
	int GetHealth() const;
	int GetScore() const;
	void AddScore(int addValue);

private:
	void StateManager(const Level* level);
	void InputManager(const Level* level);
	void Move(float elapsedSec);
	void UpdateFrame(float elapsedSec);
	int GetNrFrames();
	void SetSourceRect();
	void DrawSprite() const;
	void SetTopOfLadder(const Level* level);
	void TakeDamage(float elapsedSec, const Level* level, BulletManager* bulletManager, EnemyManager* enemyManager);
	void FlickerAvatar(float elapsedSec);

	Rectf m_DstRect;
	float m_HorSpeed;
	float m_VerSpeed;
	float m_JumpSpeed;
	Vector2f m_Acceleration;
	Vector2f m_Velocity;
	AvatarState m_State;
	Texture* m_pAvatarSpriteTexture;
	bool m_IsShooting;
	bool m_IsLookingLeft;
	bool m_IsClimbing;
	float m_TopOfLadder;

	float m_AnimTime;
	int m_AnimFrame;

	float m_IdleWidth;
	float m_IdleHeight;

	float m_JumpWidth;
	float m_JumpHeight;

	float m_ShootIdleWidth;
	float m_ShootIdleHeight;

	float m_ShootRunWidth;
	float m_ShootRunHeight;

	float m_ClimbWidth;
	float m_ClimbHeight;

	float m_DieWidth;
	float m_Dieheight;

	float m_ShootJumpWidth;
	float m_ShootJumpHeight;

	Rectf m_SrcRect;
	bool m_IsClimbingDown;

	int m_FrameCounter;
	bool m_ResetCounter;

	float m_HitCoolDown;
	float m_AccuHit;

	int m_Health;
	bool m_TakenDamage;

	float m_AccuFlicker;
	float m_FlickerCoolDown;
	float m_TotalFlickerTime;
	float m_AccuTotalFlickerTime;
	bool m_IsDrawing;

	bool m_HasPlayedDie;

	int m_Score;

	SoundEffect m_MegamanDamage;
	SoundEffect m_MegamanDie;

	bool m_IsMuted;
};