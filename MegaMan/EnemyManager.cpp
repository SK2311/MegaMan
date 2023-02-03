#include "pch.h"
#include "EnemyManager.h"
#include "Level.h"
#include "Texture.h"
#include "Avatar.h"
#include "BulletManager.h"
#include "Enemy.h"
#include "Crab.h"
#include "Hamburger.h"
#include "Rocket.h"
#include "Goblin.h"
#include "Shell.h"

EnemyManager::EnemyManager(BulletManager* pBulletManager, Avatar* avatar, const Level* level)
	: m_pTexture{ new Texture{"resources/Images/Enemies.png"} }
	, m_pExplosionTexture{ new Texture{"resources/Images/Explosion.png"} }
	, m_pBulletManager{ pBulletManager }
	, m_pAvatar{avatar}
	, m_pLevel{level}
	, m_IsHit{false}
	, m_ExplosionWidth{14.f}
	, m_ExplosionHeight{16.f}
	, m_AnimTime{0.f}
	, m_AnimFrame{0}
	, m_NrFrames{2}
	, m_EnemyHit{"resources/Sound/09 - EnemyDamage.wav"}
	, m_IsMuted{false}
{
}

EnemyManager::~EnemyManager()
{
	delete m_pTexture;
	m_pTexture = nullptr;

	delete m_pExplosionTexture;
	m_pExplosionTexture = nullptr;

	DeleteEnemies();
}

void EnemyManager::CreateEnemies()
{
	m_pEnemies.push_back(new Goblin(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 25.f, 23.f, 1, 4, 1815.f, 529.f, m_pLevel->GetBoundaries().height));

	m_pEnemies.push_back(new Rocket(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 17.f, 17.f, 1, 4, 2123.f, 500.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Rocket(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 17.f, 17.f, 1, 4, 2300.f, 500.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Rocket(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 17.f, 17.f, 1, 4, 2500.f, 500.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Rocket(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 17.f, 17.f, 1, 4, 2860.f, 600.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Rocket(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 17.f, 17.f, 1, 4, 3100.f, 600.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Rocket(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 17.f, 17.f, 1, 4, 3150.f, 610.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Rocket(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 17.f, 17.f, 1, 4, 3200.f, 590.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Rocket(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 17.f, 17.f, 1, 4, 3250.f, 600.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Rocket(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 17.f, 17.f, 1, 4, 3400.f, 810.f, m_pLevel->GetBoundaries().height));

	m_pEnemies.push_back(new Shell(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 10.f, 18.f, 4, 7, 2209.f, 562.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Shell(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 10.f, 18.f, 4, 7, 2337.f, 562.f, m_pLevel->GetBoundaries().height));

	m_pEnemies.push_back(new Crab(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 16.f, 10.f, 2, 1, 2742.f, 600.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Crab(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 16.f, 10.f, 2, 1, 2800.f, 540.f, m_pLevel->GetBoundaries().height));

	m_pEnemies.push_back(new Hamburger(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 16.f, 23.f, 2, 2, 3500.f, 1020.f, m_pLevel->GetBoundaries().height));
	m_pEnemies.push_back(new Hamburger(m_pLevel, m_pBulletManager, m_pAvatar, m_pTexture, 16.f, 23.f, 2, 2, 3550.f, 1020.f, m_pLevel->GetBoundaries().height));
}

void EnemyManager::UpdateEnemies(float elapsedSec, const bool isMuted)
{
	m_IsMuted = isMuted;

	for (Enemy* enemy : m_pEnemies)
	{
		enemy->Update(elapsedSec, m_pAvatar->GetShape(), m_IsMuted);
	}

	DeleteEnemy(elapsedSec);
	if (m_IsHit)
	{
		UpdateExplosionFrame(elapsedSec);
	}
}

void EnemyManager::DrawEnemies() const
{
	for (Enemy* enemy : m_pEnemies)
	{
		enemy->Draw();
	}

	if (m_IsHit)
	{
		DrawExplosion();
	}
}

// Delete all enemy objects at the end of the game
void EnemyManager::DeleteEnemies()
{
	for (Enemy* crab : m_pEnemies)
	{
		delete crab;
	}
	m_pEnemies.clear();
}

// Delete enemy hit by bullet or when it leaves the level
void EnemyManager::DeleteEnemy(float elapsedSec)
{
	// When shot
	if (m_pBulletManager->GetHitEnemyIndex() != -1)
	{
		m_pAvatar->AddScore(m_pEnemies[m_pBulletManager->GetHitEnemyIndex()]->GetScoreToGive());

		if (!m_IsMuted)
		{
			m_EnemyHit.Play(0);
		}

		m_DstRect = m_pEnemies[m_pBulletManager->GetHitEnemyIndex()]->GetRect();

		delete m_pEnemies[m_pBulletManager->GetHitEnemyIndex()];
		m_pEnemies.erase(m_pEnemies.begin() + m_pBulletManager->GetHitEnemyIndex());
		m_pBulletManager->SetEnemyIndex(-1);

		m_IsHit = true;
	}
	else
	{
		// When out of the level boundaries
		for (int i{}; i < m_pEnemies.size(); ++i)
		{
			if (m_pEnemies[i]->GetRect().left + m_pEnemies[i]->GetRect().width <= 0.f)
			{
				delete m_pEnemies[i];
				m_pEnemies.erase(m_pEnemies.begin() + i);
			}
		}
	}
}

// Get a vector with the Rect of every enemy object
void EnemyManager::GetRects(std::vector<Rectf>& result) const
{
	Rectf rect{};
	for (int i{}; i < m_pEnemies.size(); ++i)
	{
		rect = m_pEnemies[i]->GetRect();
		result.push_back(rect);
	}
}

bool EnemyManager::HitPlayer(const Rectf& player) const
{
	bool result{ false };
	for (int i{}; i < m_pEnemies.size(); ++i)
	{
		result = utils::IsOverlapping(m_pEnemies[i]->GetRect(), player);
		if (result)
		{
			break;
		}
	}
	return result;
}

void EnemyManager::DrawExplosion() const
{
	m_pExplosionTexture->Draw(m_DstRect, m_SrcRect);
}

void EnemyManager::UpdateExplosionFrame(float elapsedSec)
{
	if (m_AnimFrame != 1)
	{
		m_AnimTime += elapsedSec;
		float frameSec{ float(1.1 / m_NrFrames) };
		if (m_AnimTime > frameSec)
		{
			++m_AnimFrame %= m_NrFrames;
			m_AnimTime -= frameSec;
		}
		SetSourceRect();
	}
	else
	{
		m_AnimTime = 0;
		m_AnimFrame = 0;
		m_IsHit = false;
	}
}

void EnemyManager::SetSourceRect()
{
	m_SrcRect.width = m_ExplosionWidth;
	m_SrcRect.height = m_ExplosionHeight;
	m_SrcRect.left = float(m_AnimFrame % m_NrFrames) * m_ExplosionWidth;
	m_SrcRect.bottom = m_ExplosionHeight;
}
