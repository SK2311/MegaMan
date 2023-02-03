#include "pch.h"
#include "BulletManager.h"
#include "Bullet.h"
#include "Texture.h"
#include <iostream>

BulletManager::BulletManager()
	: m_pTexture{ new Texture{"resources/Images/Bullet.png"} }
	, m_CoolDownTimer{0.5f}
	, m_AccuCoolDownPlayer{0.f}
	, m_HitEnemyIndex{-1}
	, m_MegamanShoot{"resources/Sound/05 - MegamanShoot.wav"}
	, m_IsMuted{false}
{
}

BulletManager::~BulletManager()
{
	delete m_pTexture;
	m_pTexture = nullptr;

	DeleteAllBullets();
}

void BulletManager::CreateBullet(bool isShooting, const Rectf& shape, float directionX, float directionY)
{
	if (isShooting && m_AccuCoolDownPlayer >= m_CoolDownTimer)
	{
		if (!m_IsMuted)
		{
			m_MegamanShoot.Play(0);
		}		

		Point2f pos{ shape.left + shape.width, shape.bottom + shape.height / 2 };
		if (directionX == -1)
		{
			// going left
			pos.x = shape.left;
		}

		m_pBullets.push_back(new Bullet(m_pTexture, directionX, directionY, pos));
		m_AccuCoolDownPlayer = 0.f;
	}
}

// Create 1 bullet object and add to the vector
void BulletManager::CreateEnemyBullet(bool isShooting, const Rectf& shape, float directionX, float directionY)
{
	if (isShooting)
	{
		Point2f pos{ shape.left + shape.width, shape.bottom + shape.height / 2 };
		if (directionX == -1)
		{
			// going left
			pos.x = shape.left;
		}

		if (directionY == 1)
		{
			// going up
			pos.y = shape.bottom + shape.height;
		}
		else if (directionY == -1)
		{
			// going down
			pos.y = shape.bottom;
		}

		m_pEnemyBullets.push_back(new Bullet(m_pTexture, directionX, directionY, pos));
	}
}

// Update bullets shot by enemy objects
void BulletManager::UpdateBullets(float elapsedSec, const Rectf& shape, const bool isMuted)
{
	m_IsMuted = isMuted;

	for (Bullet* bullet : m_pEnemyBullets)
	{
		bullet->Update(elapsedSec);
	}

	DeleteBullet(shape);
}

// Update bullets shot by player
void BulletManager::UpdateBullets(float elapsedSec, const std::vector<Rectf>& shapes)
{
	m_AccuCoolDownPlayer += elapsedSec;

	for (Bullet* bullet : m_pBullets)
	{
		bullet->Update(elapsedSec);
	}

	DeleteBullet(shapes);
}

void BulletManager::DrawBullets() const
{
	for (Bullet* bullet : m_pEnemyBullets)
	{
		bullet->Draw();
	}

	for (Bullet* bullet : m_pBullets)
	{
		bullet->Draw();
	}
}

// Delete all bullets when the EnemyManager object gets destroyed
void BulletManager::DeleteAllBullets()
{
	for (Bullet* bullet : m_pEnemyBullets)
	{
		delete bullet;
	}
	m_pEnemyBullets.clear();

	for (Bullet* bullet : m_pBullets)
	{
		delete bullet;
	}
	m_pBullets.clear();
}

int BulletManager::GetHitEnemyIndex() const
{
	return m_HitEnemyIndex;
}

void BulletManager::SetEnemyIndex(int index)
{
	m_HitEnemyIndex = index;
}

// Delete bullet that has reached max time limit or that has hit the player
void BulletManager::DeleteBullet(const Rectf& shape)
{
	for (int i{}; i < m_pEnemyBullets.size(); ++i)
	{
		if (m_pEnemyBullets[i]->IsTimeUp() || m_pEnemyBullets[i]->HitDetection(shape))
		{
			delete m_pEnemyBullets[i];
			m_pEnemyBullets.erase(m_pEnemyBullets.begin() + i);
		}
	}
}

// Delete bullet that has reached max time limit or that has hit an enemy
void BulletManager::DeleteBullet(const std::vector<Rectf>& shapes)
{
	for (int i{}; i < m_pBullets.size(); ++i)
	{
		if (m_pBullets[i]->IsTimeUp() || m_pBullets[i]->HitDetection(shapes))
		{
			m_HitEnemyIndex = m_pBullets[i]->GetHitEnemyIndex();
			delete m_pBullets[i];
			m_pBullets.erase(m_pBullets.begin() + i);
		}
	}
}

bool BulletManager::HitDetection(const Rectf& shape) const
{
	bool result{ false };
	for (int i{}; i < m_pEnemyBullets.size(); ++i)
	{
		if (m_pEnemyBullets[i]->HitDetection(shape))
		{
			result = true;
			break;
		}
	}
	return result;
}
