#include "pch.h"
#include "Bullet.h"
#include "Texture.h"

Bullet::Bullet(Texture* texture, float directionX, float directionY, Point2f pos)
	: m_pTexture{ texture }
	, m_DirectionMultiplierX{directionX}
	, m_DirectionMultiplierY{directionY}
	, m_StartPos{pos}
	, m_BulletSpeed{150.f}
	, m_MaxTimeAlive{1.f}
	, m_TimeAlive{0.f}
	, m_HitEnemyIndex{-1}
{
	m_DstRect = Rectf{ m_StartPos.x,m_StartPos.y,m_pTexture->GetWidth(),m_pTexture->GetHeight() };
	m_Velocity = Vector2f{ m_BulletSpeed * m_DirectionMultiplierX, m_BulletSpeed * m_DirectionMultiplierY };
}

void Bullet::Draw() const
{
	m_pTexture->Draw(m_DstRect);
}

void Bullet::Update(float elapsedSec)
{
	m_TimeAlive += elapsedSec;

	Move(elapsedSec);
}

bool Bullet::HitDetection(const Rectf& shape) const
{
	return utils::IsOverlapping(m_DstRect, shape);
}

bool Bullet::HitDetection(const std::vector<Rectf> shapes)
{
	bool result{ false };
	for (int i{}; i < shapes.size(); ++i)
	{
		if (utils::IsOverlapping(m_DstRect, shapes[i]))
		{
			m_HitEnemyIndex = i;
			result = true;
			break;
		}
	}
	return result;
}

bool Bullet::IsTimeUp()
{
	return m_TimeAlive >= m_MaxTimeAlive;
}

int Bullet::GetHitEnemyIndex() const
{
	return m_HitEnemyIndex;
}

Rectf Bullet::GetRect() const
{
	return m_DstRect;
}

void Bullet::Move(float elapsedSec)
{
	m_DstRect.left += m_Velocity.x * elapsedSec;
	m_DstRect.bottom += m_Velocity.y * elapsedSec;
}
