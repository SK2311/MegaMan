#include "pch.h"
#include "Rocket.h"
#include "Texture.h"
#include <iostream>

Rocket::Rocket(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight)
	:Enemy{level, bulletManager, avatar, texture, width, height, nrFrames, row, xPos, yPos, levelHeight}
	, m_HorizontalSpeed{70.f}
	, m_VerticalSpeed{50.f}
	, m_Angle{0}
{
	SetColliding(false);
	SetAnimation(false);

	m_ScoreToGive = 800;
}

void Rocket::Update(float elapsedSec, const Rectf& player, const bool isMuted)
{
	if (Enemy::IsPlayerInRange(player))
	{
		Enemy::Update(elapsedSec, player, isMuted);

		Move(elapsedSec);
	}	
}

void Rocket::Move(float elapsedSec)
{
	m_Angle += elapsedSec;
	if (m_Angle >= 360)
	{
		m_Angle = 0;
	}

	m_Velocity.x = -m_HorizontalSpeed;
	m_Velocity.y = m_VerticalSpeed * sinf(m_Angle);
	m_DstRect.left += m_Velocity.x * elapsedSec;
	m_DstRect.bottom += m_Velocity.y * elapsedSec;
}
