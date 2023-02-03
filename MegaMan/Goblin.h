#pragma once
#include "Enemy.h"
class Goblin final : public Enemy
{
public:
	explicit Goblin(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight = 0.f);
	Goblin(const Goblin& other) = delete;
	Goblin& operator=(const Goblin& other) = delete;
	Goblin(Goblin&& other) = delete;
	Goblin& operator=(Goblin&& other) = delete;
	~Goblin() = default;

	virtual void Update(float elapsedSec, const Rectf& player, const bool isMuted) override;

private:
	void Move(float elapsedSec, BulletManager* bulletManager, const Avatar* avatar, const Level* level);
	void SetGoblinSourceRect(bool jumping);
	void Shoot(BulletManager* bulletManager, const Avatar* avatar) const;
	void Jump();

	bool m_HasSHot;
	float m_AccuTime;
	float m_CoolDown;

	float m_AccuTimeJump;
	float m_TimeBeforeJump;

	float m_VerSpeed;
	float m_PlayerPosX;
	float m_PosX;
	float m_Direction;
};

