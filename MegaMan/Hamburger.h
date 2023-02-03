#pragma once
#include "Enemy.h"
class Hamburger final : public Enemy
{
public:
	explicit Hamburger(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight = 0.f);
	Hamburger(const Hamburger& other) = delete;
	Hamburger& operator=(const Hamburger& other) = delete;
	Hamburger(Hamburger&& other) = delete;
	Hamburger& operator=(Hamburger&& other) = delete;
	~Hamburger() = default;

	virtual void Update(float elapsedSec, const Rectf& player, const bool isMuted) override;
	
private:
	void Move(float elapsedSec);
	bool DetectPlayer(const Rectf& player) const;
	void SetDetectionRect();
	void Shoot(float elapsedSec, BulletManager* bulletManager);

	float m_HorizontalSpeed;
	float m_DetectionOffset;
	float m_ShootCoolDown;
	float m_AccuCoolDown;
};