#pragma once
#include <vector>

class Texture;
struct Vector2f;

class Level final
{
public:
	explicit Level();
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level && other) = delete;
	Level& operator=(Level&& other) = delete;
	~Level();

	void DrawLevel() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, float ladderY, bool isClimbingDown) const;
	bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity, float ladderY) const;
	Rectf GetBoundaries() const;
	bool IsOverlappingWithLadder(const Rectf& actorShape) const;
	float GetYTopOfLadder(const Rectf& actorShape) const;
	Point2f GetXTopOfLadder(const Rectf& actorShape) const;
	bool SpikesHitDetection(const Rectf& player) const;
	Texture* GetBackground() const;
	Texture* GetMiddleground() const;

private:
	std::vector<Rectf> SetSpikes() const;

	Texture* m_pLevelTexture;
	Texture* m_pBackGround;
	Texture* m_pMiddleGround;
	std::string m_SVGPath;
	std::string m_LadderSVGPath;
	Rectf m_Boundaries;

	std::vector<std::vector<Point2f>>m_Vertices;
	std::vector<std::vector<Point2f>>m_VerticesLadder;
	std::vector<Rectf> m_Spikes;
};