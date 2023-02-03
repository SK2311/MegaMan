#pragma once
class Texture;

class HUD
{
public:
	explicit HUD(const Point2f& topLeftHealth, const Point2f& topLeftScore);
	HUD(const HUD& other) = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD(HUD&& other) = delete;
	HUD& operator=(HUD&& other) = delete;
	~HUD();

	void Update(int playerHealth, int playerScore);
	void Draw() const;

private:
	int m_PlayerHealth;
	int m_PlayerScore;
	Texture* m_pHealthBar;
	Rectf m_EmptyHealthBarSrc;
	Rectf m_FullHealthBarSrc;
	Rectf m_DstRectEmptyBar;
	Rectf m_DstRectFullBar;
	const int m_NumbersTextureSize;
	Texture* m_NumbersTexture;
	Color4f m_NumberColour;
	std::string m_FontPath;
	Rectf m_DstRectScore;

	void UpdateSrcRect();
	void InitializeNumbersTexture();
};