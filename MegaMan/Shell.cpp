#include "pch.h"
#include "Shell.h"
#include "BulletManager.h"
#include <iostream>

Shell::Shell(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight)
	: Enemy{level, bulletManager, avatar, texture, width, height, nrFrames, row, xPos, yPos, levelHeight}
	, m_DetectionOffset{ 100.f }
	, m_ShootCoolDown{ 2.f }
	, m_AccuCoolDown{ 0.f }
	, m_FrameNr{0}
	, m_DstCounter{0}
	, m_ChangeLeft{true}
{
	m_DstRect.left -= m_DstRect.width;
	SetColliding(false);
	SetAnimation(false);
	SetFrameSec(5);

	m_ScoreToGive = 200;
}

void Shell::Update(float elapsedSec, const Rectf& player, const bool isMuted)
{
	SetDetectionRect();
	
	SetShellSrc();

	if (Enemy::IsPlayerInRange(player))
	{
		Enemy::Update(elapsedSec, player, isMuted);
		SetSrc(false);
		if (DetectPlayer(player))
		{
			SetDstRect();
			if (m_AnimFrame == 3)
			{
				SetAnimation(false);
				Shoot(elapsedSec, m_pBulletManager);
			}
			else
			{
				SetAnimation(true);
			}
			m_FrameNr = m_AnimFrame;
		}
	}
	m_AccuCoolDown += elapsedSec;
}

void Shell::SetDstRect()
{
	if (m_FrameNr == 0)
	{
		m_DstRect.width = 10.f;
	}
	else if (m_FrameNr == 1)
	{
		m_DstRect.width = 8.f;
	}
	else if (m_FrameNr == 2)
	{
		m_DstRect.width = 11.f;
	}
	else if (m_FrameNr == 3)
	{
		m_DstRect.width = 15.f;
	}
}

void Shell::SetShellSrc()
{
	if (m_AnimFrame == 1)
	{
		m_SrcRect.left = 16.f;
		m_SrcRect.width = 8.f;
	}
	else if (m_AnimFrame == 2)
	{
		m_SrcRect.left = 33.f;
		m_SrcRect.width = 11.f;
	}
	else if (m_AnimFrame == 3)
	{
		m_SrcRect.left = 60.f;
		m_SrcRect.width = 15.f;
	}
}

bool Shell::DetectPlayer(const Rectf& player) const
{
	return utils::IsOverlapping(m_DetectionRect, player);
}

void Shell::SetDetectionRect()
{
	Point2f center{ m_DstRect.left + m_DstRect.width / 2, m_DstRect.bottom + m_DstRect.height / 2 };
	m_DetectionRect = Rectf{ center.x - m_DetectionOffset, center.y - m_DetectionOffset, 2 * m_DetectionOffset, 2 * m_DetectionOffset };
}

void Shell::Shoot(float elapsedSec, BulletManager* bulletManager)
{
	if (m_AccuCoolDown >= m_ShootCoolDown)
	{
		if (!m_IsMuted)
		{
			m_EnemyShoot.Play(0);
		}

		bulletManager->CreateEnemyBullet(true, m_DstRect, -1, 0);
		m_AccuCoolDown = 0.f;
	}
}