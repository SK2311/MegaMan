#include "pch.h"
#include "Hamburger.h"
#include <iostream>
#include "BulletManager.h"

Hamburger::Hamburger(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight)
	: Enemy{level, bulletManager, avatar, texture, width, height, nrFrames, row, xPos, yPos, levelHeight}
	, m_HorizontalSpeed{ 50.f }
	, m_DetectionOffset{100.f}
	, m_ShootCoolDown{2.f}
	, m_AccuCoolDown{ 0.f }
{
	SetColliding(false);
	SetAnimation(false);
	SetFrameSec(2);

	m_ScoreToGive = 800;
}

void Hamburger::Update(float elapsedSec, const Rectf& player, const bool isMuted)
{
	SetDetectionRect();

	if (Enemy::IsPlayerInRange(player))
	{
		Enemy::Update(elapsedSec, player, isMuted);
		
		if (m_AnimFrame != 1)
		{
			Move(elapsedSec);
		}
		else
		{
			Shoot(elapsedSec, m_pBulletManager);
			if (!DetectPlayer(player))
			{
				Move(elapsedSec);
			}
		}

		if (DetectPlayer(player))
		{
			SetAnimation(true);
		}
		else
		{
			SetAnimation(false);
		}
	}

	m_AccuCoolDown += elapsedSec;
}

void Hamburger::Move(float elapsedSec)
{
	m_Velocity.x = -m_HorizontalSpeed;
	m_DstRect.left += m_Velocity.x * elapsedSec;
}

bool Hamburger::DetectPlayer(const Rectf& player) const
{
	return utils::IsOverlapping(m_DetectionRect, player);
}

void Hamburger::SetDetectionRect()
{
	Point2f center{ m_DstRect.left + m_DstRect.width / 2, m_DstRect.bottom + m_DstRect.height / 2 };
	m_DetectionRect = Rectf{ center.x - m_DetectionOffset, center.y - m_DetectionOffset, 2 * m_DetectionOffset, 2 * m_DetectionOffset };
}

void Hamburger::Shoot(float elapsedSec, BulletManager* bulletManager)
{
	if (m_AccuCoolDown >= m_ShootCoolDown)
	{
		if (!m_IsMuted)
		{
			m_EnemyShoot.Play(0);
		}

		// create bullet objects
		bulletManager->CreateEnemyBullet(true, m_DstRect, 1.f, 0.f); // going right
		bulletManager->CreateEnemyBullet(true, m_DstRect, 1.f, -1.f); // going bottom right
		bulletManager->CreateEnemyBullet(true, m_DstRect, 0.f, -1.f); // going down
		bulletManager->CreateEnemyBullet(true, m_DstRect, -1.f, -1.f); // going bottom left
		bulletManager->CreateEnemyBullet(true, m_DstRect, -1.f, 0.f); // going left
		bulletManager->CreateEnemyBullet(true, m_DstRect, -1.f, 1.f); // going top left
		bulletManager->CreateEnemyBullet(true, m_DstRect, 0.f, 1.f); // going up
		bulletManager->CreateEnemyBullet(true, m_DstRect, 1.f, 1.f); // going top right

		m_AccuCoolDown = 0.f;
	}
}