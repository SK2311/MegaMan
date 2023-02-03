#pragma once
#include "SoundEffect.h"

class Texture;
class Level;
class Avatar;
class BulletManager;
class Enemy;

class EnemyManager final
{
public:
	explicit EnemyManager(BulletManager* pBulletManager, Avatar* avatar, const Level* level);
	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager& operator=(const EnemyManager& other) = delete;
	EnemyManager(EnemyManager&& other) = delete;
	EnemyManager& operator=(EnemyManager&& other) = delete;
	~EnemyManager();

	void CreateEnemies();
	void UpdateEnemies(float elapsedSec, const bool isMuted);
	void DrawEnemies() const;
	void DeleteEnemies();
	void DeleteEnemy(float elapsedSec);
	void GetRects(std::vector<Rectf>& result) const;
	bool HitPlayer(const Rectf& player) const;

private:
	void DrawExplosion() const;
	void UpdateExplosionFrame(float elapsedSec);
	void SetSourceRect();

	Texture* m_pTexture;
	Texture* m_pExplosionTexture;
	BulletManager* m_pBulletManager;
	Avatar* m_pAvatar;
	const Level* m_pLevel;
	std::vector<Enemy*> m_pEnemies;
	bool m_IsHit;
	Rectf m_DstRect;
	Rectf m_SrcRect;

	float m_ExplosionWidth;
	float m_ExplosionHeight;
	float m_AnimTime;
	int m_AnimFrame;
	int m_NrFrames;

	SoundEffect m_EnemyHit;

	bool m_IsMuted;
};

