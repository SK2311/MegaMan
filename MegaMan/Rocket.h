#pragma once
#include "Enemy.h"

class Rocket final : public Enemy
{
public:
	explicit Rocket(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight = 0.f);
	Rocket(const Rocket& other) = delete;
	Rocket& operator=(const Rocket& other) = delete;
	Rocket(Rocket&& other) = delete;
	Rocket& operator=(Rocket&& other) = delete;
	~Rocket() = default;

	virtual void Update(float elapsedSec, const Rectf& player, const bool isMuted) override;
private:
	void Move(float elapsedSec);

	float m_HorizontalSpeed;
	float m_VerticalSpeed;
	float m_Angle;
};

