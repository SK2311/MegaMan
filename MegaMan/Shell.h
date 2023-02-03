#pragma once
#include "Enemy.h"
class Shell final : public Enemy
{
public:
	explicit Shell(const Level* level, BulletManager* bulletManager, const Avatar* avatar, Texture* texture, float width, float height, int nrFrames, int row, float xPos, float yPos, float levelHeight = 0.f);
	Shell(const Shell& other) = delete;
	Shell& operator=(const Shell& other) = delete;
	Shell(Shell&& other) = delete;
	Shell& operator=(Shell&& other) = delete;
	~Shell() = default;

	virtual void Update(float elapsedSec, const Rectf& player, const bool isMuted) override;

private:
	void SetDstRect();
	void SetShellSrc();
	bool DetectPlayer(const Rectf& player) const;
	void SetDetectionRect();
	void Shoot(float elapsedSec, BulletManager* bulletManager);

	float m_DetectionOffset;
	float m_ShootCoolDown;
	float m_AccuCoolDown;

	int m_FrameNr;
	int m_DstCounter;
	bool m_ChangeLeft;
};

