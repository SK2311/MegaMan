#pragma once
#include "SoundEffect.h"

class Texture;
struct Vector2f;
class Level;
class BulletManager;
class Avatar;

class Enemy
{
public:
	explicit Enemy(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight = 0.f);
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& other) = delete;
	virtual ~Enemy() = default;

	virtual void Draw() const;
	virtual void Update(float elapsedSec, const Rectf& player, const bool isMuted);
	Rectf GetRect() const;
	int GetScoreToGive() const;

protected:
	Rectf m_SrcRect;
	Rectf m_DstRect;
	Rectf m_BorderRect; // Rect to determine if the player is close to the enemy
	Vector2f m_Velocity;
	Texture* m_pTexture;

	const Level* m_pLevel;
	BulletManager* m_pBulletManager;
	const Avatar* m_pAvatar;
	
	float m_AnimTime;
	int m_AnimFrame;
	int m_NrFrames;
	float m_Width;
	float m_Height;
	int m_Row;

	float m_XPos;
	float m_YPos;

	int m_ScoreToGive;

	float m_LevelHeight;

	Vector2f m_Acceleration;
	Rectf m_DetectionRect;

	SoundEffect m_EnemyShoot;

	bool m_IsMuted;

	bool IsPlayerInRange(const Rectf& player) const;
	void SetColliding(bool isColliding);
	void SetAnimation(bool isAnimating);
	void SetFrameSec(int sec);
	void SetSrc(bool set);
	void SetDirection(bool lookingLeft);

private:
	void SetSourceRect(bool isAnimating);
	void UpdateBorderRect();
	void UpdateDirection() const;

	bool m_IsCollisionActive;
	bool m_IsAnimating;

	float m_BorderOffset;
	int m_Sec; // Integer for variable frameSec

	bool m_ResetCounter;
	int m_FrameCounter;
	bool m_Hit;

	bool m_SetSrc;
	bool m_LookingLeft;
};

