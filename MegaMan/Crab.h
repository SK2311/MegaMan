#pragma once
#include "Enemy.h"

class Crab final : public Enemy
{
public:
	explicit Crab(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight = 0.f);
	Crab(const Crab& other) = delete;
	Crab& operator=(const Crab& other) = delete;
	Crab(Crab&& other) = delete;
	Crab& operator=(Crab&& other) = delete;
	~Crab() = default;

	virtual void Update(float elapsedSec, const Rectf& player, const bool isMuted) override;

private:
	void Move(float elapsedSec, float xPos);

	Rectf m_MovementRect;
	float m_HorizontalSpeed;
	bool m_ShouldMovingLeft;
};

