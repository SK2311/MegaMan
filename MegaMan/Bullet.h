#pragma once
class Texture;
struct Vector2f;

class Bullet final
{
public:
	explicit Bullet(Texture* texture, float directionX, float directionY, Point2f pos);
	Bullet(const Bullet& other) = delete;
	Bullet& operator=(const Bullet& other) = delete;
	Bullet (Bullet && other) = delete;
	Bullet& operator=(Bullet&& other) = delete;
	~Bullet() = default;

	void Draw() const;
	void Update(float elapsedSec);
	bool HitDetection(const Rectf& shape) const;
	bool HitDetection(const std::vector<Rectf> shapes);
	bool IsTimeUp();
	int GetHitEnemyIndex() const;
	Rectf GetRect() const;

private:
	void Move(float elapsedSec);

	Texture* m_pTexture;	
	float m_DirectionMultiplierX; // 1 for going right, -1 for going left
	float m_DirectionMultiplierY; // 1 for going up, -1 for going down
	Point2f m_StartPos;
	float m_BulletSpeed;
	float m_MaxTimeAlive;
	float m_TimeAlive;
	
	Rectf m_DstRect;	
	Vector2f m_Velocity;

	int m_HitEnemyIndex;
};

