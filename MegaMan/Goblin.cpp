#include "pch.h"
#include "Goblin.h"
#include "BulletManager.h"
#include "Avatar.h"
#include "Level.h"
#include <iostream>

Goblin::Goblin(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight)
	: Enemy{ level, bulletManager, avatar, texture, width, height, nrFrames, row, xPos, yPos, levelHeight }
	, m_HasSHot{false}
	, m_AccuTime{0.f}
	, m_CoolDown{2.f}
	, m_AccuTimeJump{0.f}
	, m_TimeBeforeJump{0.3f}
	, m_VerSpeed{350.f}
	, m_PlayerPosX{}
	, m_PosX{}
	, m_Direction{}
{
	SetColliding(true);
	SetAnimation(false);
	SetSrc(true);

	m_ScoreToGive = 5000;
}

void Goblin::Update(float elapsedSec, const Rectf& player, const bool isMuted)
{
	if (Enemy::IsPlayerInRange(player))
	{
		Enemy::Update(elapsedSec, player, isMuted);
		SetSrc(false);

		m_PlayerPosX = m_pAvatar->GetShape().left + m_pAvatar->GetShape().width / 2;
		m_PosX = m_DstRect.left + m_DstRect.width / 2;
		if (m_PlayerPosX >= m_PosX)
		{
			// player is to the right of enemy
			m_Direction = 1.f;
			SetDirection(true);
		}
		else
		{
			m_Direction = -1.f;
			SetDirection(false);
		}

		Move(elapsedSec, m_pBulletManager, m_pAvatar, m_pLevel);
	}
}

void Goblin::Move(float elapsedSec, BulletManager* bulletManager, const Avatar* avatar, const Level* level)
{
	m_AccuTime += elapsedSec;
	
	if (m_AccuTime >= m_CoolDown)
	{
		if (!m_HasSHot)
		{
			Shoot(bulletManager, avatar);
			m_HasSHot = true;
		}

		m_AccuTimeJump += elapsedSec;

		if (m_AccuTimeJump >= m_TimeBeforeJump)
		{
			Jump();
			m_HasSHot = false;
			m_AccuTimeJump = 0.f;
			m_AccuTime = 0.f;
		}
	}

	if (level->IsOnGround(m_DstRect, m_Velocity, 0))
	{
		SetGoblinSourceRect(false);
	}
	else
	{
		SetGoblinSourceRect(true);
	}
	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_DstRect.bottom += m_Velocity.y * elapsedSec;
}

void Goblin::SetGoblinSourceRect(bool jumping)
{
	if (jumping)
	{
		m_SrcRect.left = 28.f;
		m_SrcRect.bottom = 93.f;
		m_SrcRect.width = 28.f;
		m_SrcRect.height = 31.f;
	}
	else
	{
		m_SrcRect.left = 0.f;
		m_SrcRect.bottom = m_Row * m_Height;
		m_SrcRect.width = 25.f;
		m_SrcRect.height = 23.f;
	}
}

void Goblin::Shoot(BulletManager* bulletManager, const Avatar* avatar) const
{
	if (!m_IsMuted)
	{
		m_EnemyShoot.Play(0);
	}
	bulletManager->CreateEnemyBullet(true, m_DstRect, m_Direction, 0.f);
}

void Goblin::Jump()
{
	m_Velocity.y = m_VerSpeed;
}
