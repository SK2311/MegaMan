#include "pch.h"
#include "HUD.h"
#include "Texture.h"

HUD::HUD(const Point2f& topLeftHealth, const Point2f& topLeftScore)
	: m_PlayerHealth{}
	, m_PlayerScore{}
	, m_pHealthBar{ new Texture{"resources/Images/HealthBar.png"} }
	, m_EmptyHealthBarSrc{ 8.f,0.f,8.f,41.f }
	, m_FullHealthBarSrc{ 0.f,0.f,8.f,41.f }
	, m_DstRectEmptyBar{}
	, m_NumbersTextureSize{10}
	, m_NumberColour{1.f,1.f,1.f,1.f}
	, m_FontPath{"resources/Font/MMRock9.ttf"}
{
	m_DstRectEmptyBar = Rectf{ topLeftHealth.x, topLeftHealth.y - (m_EmptyHealthBarSrc.height * 2),
									m_EmptyHealthBarSrc.width * 2, m_EmptyHealthBarSrc.height * 2 };
	m_DstRectFullBar = Rectf{ topLeftHealth.x, topLeftHealth.y - (m_FullHealthBarSrc.height * 2),
									m_FullHealthBarSrc.width * 2, m_FullHealthBarSrc.height * 2 };
	InitializeNumbersTexture();
	m_DstRectScore = Rectf{ topLeftScore.x, topLeftScore.y 
											- m_NumbersTexture->GetHeight(), 
											m_NumbersTexture->GetWidth() / 10, 
											m_NumbersTexture->GetHeight() };
}

HUD::~HUD()
{
	delete m_pHealthBar;
	m_pHealthBar = nullptr;

	delete m_NumbersTexture;
	m_NumbersTexture = nullptr;
}

void HUD::Update(int playerHealth, int playerScore)
{
	m_PlayerHealth = playerHealth;
	m_PlayerScore = playerScore;

	UpdateSrcRect();
}

void HUD::UpdateSrcRect()
{
	float originalHeight{ 80.f };
	int hitCounter{ 10 - m_PlayerHealth };

	m_DstRectFullBar.height = originalHeight - float(hitCounter * 8);
	m_FullHealthBarSrc.height = originalHeight - float(hitCounter * 8);
}

void HUD::InitializeNumbersTexture()
{
	m_NumbersTexture = new Texture{ std::string{"0123456789"}, m_FontPath, 20, m_NumberColour };
}

void HUD::Draw() const
{
	// Draw health bar
	m_pHealthBar->Draw(m_DstRectEmptyBar, m_EmptyHealthBarSrc);
	if (m_PlayerHealth > 0)
	{
		m_pHealthBar->Draw(m_DstRectFullBar, m_FullHealthBarSrc);
	}

	Rectf dstRect{ m_DstRectScore };
	Rectf srcRect{ 0.f, 0.f, m_NumbersTexture->GetWidth() / 10, m_NumbersTexture->GetHeight() };
	// Draw score
	int digits{ int(log10((float)m_PlayerScore) + 1) };
	int nrOfZeroes{ 7 - digits };

	int divider{};
	if (digits > 0)
	{
		divider = int(pow(10, digits - 1));
	}

	int tempScore{ m_PlayerScore };
	for (int i{}; i < 7; ++i)
	{
		srcRect.left = 0.f;
		if (i < nrOfZeroes || digits < 0 || tempScore <= 0)
		{
			m_NumbersTexture->Draw(dstRect, srcRect);
			dstRect.left += m_NumbersTexture->GetWidth() / 10;
		}
		else
		{
			int digit{ tempScore / divider };

			srcRect.left += (m_NumbersTexture->GetWidth() / 10) * digit;
			m_NumbersTexture->Draw(dstRect, srcRect);
			dstRect.left += m_NumbersTexture->GetWidth() / 10;
			tempScore -= digit * divider;

			digits = int(log10((float)tempScore) + 1);
			divider = int(pow(10, digits - 1));
		}
	}
}