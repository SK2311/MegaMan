#pragma once
#include "SoundEffect.h"

class Texture;
class Bullet;

class BulletManager final
{
public:
	explicit BulletManager();
	BulletManager(const BulletManager& other) = delete;
	BulletManager& operator=(const BulletManager& other) = delete;
	BulletManager(BulletManager&& other) = delete;
	BulletManager& operator=(BulletManager && other) = delete;
	~BulletManager();


	void CreateBullet(bool isShooting, const Rectf& shape, float directionX, float directionY);
	void CreateEnemyBullet(bool isShooting, const Rectf& shape, float directionX, float directionY);
	void UpdateBullets(float elapsedSec, const Rectf& shape, const bool isMuted);
	void UpdateBullets(float elapsedSec, const std::vector<Rectf>& shapes);
	void DrawBullets() const;
	void DeleteAllBullets();
	int GetHitEnemyIndex() const;
	void SetEnemyIndex(int index);
	bool HitDetection(const Rectf& shape) const;

private:
	void DeleteBullet(const Rectf& shape);
	void DeleteBullet(const std::vector<Rectf>& shapes);
	

	Texture* m_pTexture;
	float m_CoolDownTimer;
	float m_AccuCoolDownPlayer;
	int m_HitEnemyIndex;

	std::vector<Bullet*> m_pBullets;
	std::vector<Bullet*> m_pEnemyBullets;

	SoundEffect m_MegamanShoot;
	bool m_IsMuted;
};