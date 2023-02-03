#pragma once
class Texture;

class Camera final
{
public:
	explicit Camera(float width, float height, float scaleFactor);
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;
	Camera(Camera&& other) = delete;
	Camera& operator=(Camera && other) = delete;
	~Camera() = default;

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Translate(const Rectf& target, const Texture* bg, const Texture* mg) const;

private:
	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;

	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;

	const float m_ScaleFactor;
};

