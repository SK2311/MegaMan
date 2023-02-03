#include "pch.h"
#include "Enemy.h"
#include "Texture.h"
#include "Level.h"
#include <iostream>

Enemy::Enemy(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight)
	: m_SrcRect{}
	, m_Velocity{0.f,0.f}
	, m_pTexture{texture}
	, m_pLevel{level}
	, m_pBulletManager{bulletManager}
	, m_pAvatar{avatar}
	, m_AnimTime{ 0.f }
	, m_AnimFrame{ 0 }
	, m_NrFrames{ nrFrames }
	, m_Width{width}
	, m_Height{height}
	, m_Row{row}
	, m_XPos{xPos}
	, m_YPos{yPos}
	, m_ScoreToGive{}
	, m_LevelHeight{levelHeight}
	, m_Acceleration{ 0.f, -981.f }
	, m_DetectionRect{}
	, m_IsCollisionActive{true}
	, m_IsAnimating{true}
	, m_BorderOffset{500.f}
	, m_Sec{1}
	, m_ResetCounter{false}
	, m_FrameCounter{0}
	, m_Hit{false}
	, m_SetSrc{true}
	, m_LookingLeft{false}
	, m_EnemyShoot{ "resources/Sound/10 - EnemyShoot.wav" }
	, m_IsMuted{false}
{
	m_DstRect = Rectf(m_XPos, m_YPos, m_Width, m_Height);
	m_BorderRect = Rectf(m_XPos - m_BorderOffset, 0.f, 2 * m_BorderOffset, m_LevelHeight);
}

void Enemy::Draw() const
{
	glPushMatrix();	
	UpdateDirection();
	m_pTexture->Draw(Rectf{ 0,0.f,m_DstRect.width, m_DstRect.height }, m_SrcRect);
	glPopMatrix();
}

void Enemy::Update(float elapsedSec,  const Rectf& player, const bool isMuted)
{
	m_IsMuted = isMuted;

	if (!m_Hit)
	{
		UpdateBorderRect();

		if (m_IsCollisionActive)
		{
			m_pLevel->HandleCollision(m_DstRect, m_Velocity, 0.f, false);
		}

		if (m_IsAnimating)
		{
			m_AnimTime += elapsedSec;
			float frameSec{ float(m_Sec / m_NrFrames) };
			if (m_AnimTime > frameSec)
			{
				++m_AnimFrame %= m_NrFrames;
				m_AnimTime -= frameSec;
			}
		}

		if (m_SetSrc)
		{
			SetSourceRect(m_IsAnimating);
		}
	}
}

Rectf Enemy::GetRect() const
{
	return m_DstRect;
}

int Enemy::GetScoreToGive() const
{
	return m_ScoreToGive;
}

void Enemy::SetSourceRect(bool isAnimating)
{
	m_SrcRect.width = m_Width;
	m_SrcRect.height = m_Height;
	m_SrcRect.bottom = m_Row * m_Height;
	if (isAnimating)
	{
		m_SrcRect.left = float(m_AnimFrame % m_NrFrames) * m_Width;
	}
	else
	{
		m_SrcRect.left = 0.f;
	}
}

void Enemy::UpdateBorderRect()
{
	Point2f pos{ m_DstRect.left + m_DstRect.width / 2, m_DstRect.bottom + m_DstRect.height / 2 };
	m_BorderRect = Rectf(pos.x - m_BorderOffset, 0.f, 2 * m_BorderOffset, m_LevelHeight);
}

void Enemy::UpdateDirection() const
{
	glTranslatef(m_DstRect.left, m_DstRect.bottom, 0);

	if (m_LookingLeft)
	{
		glScalef(-1, 1, 0);
		glTranslatef(-m_DstRect.width, 0, 0);
	}
	else
	{
		glScalef(1, 1, 0);
	}
}

// Bool to see if the player is in range in order to do the update function of the enemy types
bool Enemy::IsPlayerInRange(const Rectf& player) const
{
	Point2f playerPos{ player.left + player.width / 2, player.bottom + player.height / 2 };
	return utils::IsPointInRect(playerPos, m_BorderRect);
}

void Enemy::SetColliding(bool isColliding)
{
	m_IsCollisionActive = isColliding;
}

void Enemy::SetAnimation(bool isAnimating)
{
	m_IsAnimating = isAnimating;
}

void Enemy::SetFrameSec(int sec)
{
	m_Sec = sec;
}

void Enemy::SetSrc(bool set)
{
	m_SetSrc = set;
}

void Enemy::SetDirection(bool lookingLeft)
{
	m_LookingLeft = lookingLeft;
}
