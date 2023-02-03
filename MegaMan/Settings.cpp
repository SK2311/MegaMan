#include "pch.h"
#include "Settings.h"
#include "Texture.h"

Settings::Settings()
	: m_Menu{ new Texture {"resources/Images/settings.png"}}
	, m_MuteBar{new Texture{"resources/Images/muteBar.png"}}
	, m_CloseRect{853.f,406.f,34.f,34.f}
	, m_MuteRect{456.f,230.f,46.f,40.f}
{
}

Settings::~Settings()
{
	delete m_Menu;
	m_Menu = nullptr;

	delete m_MuteBar;
	m_MuteBar = nullptr;
}

void Settings::Draw(const bool isMuted) const
{
	m_Menu->Draw(Rectf{ 100.f,50.f,800.f,400.f });
	if (isMuted)
	{
		m_MuteBar->Draw(m_MuteRect);
	}
}

bool Settings::CloseSettingsMenu(Point2f mousePos)
{
	if (!utils::IsPointInRect(mousePos, m_CloseRect))
	{
		return true;
	}
	return false;
}

bool Settings::MuteSound(Point2f mousePos)
{
	if (utils::IsPointInRect(mousePos, m_MuteRect))
	{
		return true;
	}
	return false;
}
