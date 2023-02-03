#include "pch.h"
#include "Crab.h"
#include "Texture.h"
#include <iostream>

Crab::Crab(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight)
	: Enemy{ level, bulletManager, avatar, texture, width, height, nrFrames, row, xPos, yPos, levelHeight }
	, m_MovementRect{ Rectf{m_DstRect.left + m_DstRect.width / 2 - 50.f, 0.f, 100.f, levelHeight} }
	, m_HorizontalSpeed{75.f}
	, m_ShouldMovingLeft{false}
{
	SetColliding(true);
	SetAnimation(true);
	SetFrameSec(1);

	m_ScoreToGive = 200;
}

void Crab::Update(float elapsedSec, const Rectf& player, const bool isMuted)
{
	if (Enemy::IsPlayerInRange(player))
	{
		Enemy::Update(elapsedSec, player, isMuted);

		float xPos{ m_DstRect.left + m_DstRect.width / 2 };
		Move(elapsedSec, xPos);
	}
}

void Crab::Move(float elapsedSec, float xPos)
{
	if (xPos >= m_MovementRect.left + m_MovementRect.width)
	{
		m_ShouldMovingLeft = true;
	}
	else if (xPos <= m_MovementRect.left)
	{
		m_ShouldMovingLeft = false;
	}

	if (m_ShouldMovingLeft)
	{
		m_Velocity.x = -m_HorizontalSpeed;
	}
	else
	{
		m_Velocity.x = m_HorizontalSpeed;
	}

	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_DstRect.left += m_Velocity.x * elapsedSec;
	m_DstRect.bottom += m_Velocity.y * elapsedSec;
}
