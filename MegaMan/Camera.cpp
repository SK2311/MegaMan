#include "pch.h"
#include "Camera.h"
#include "Texture.h"
#include <iostream>

Camera::Camera(float width, float height, float scaleFactor)
	: m_ScaleFactor{ scaleFactor }
{
	m_Width = width / m_ScaleFactor;
	m_Height = height / m_ScaleFactor;
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Translate(const Rectf& target, const Texture* bg, const Texture* mg) const
{
	glScalef(m_ScaleFactor, m_ScaleFactor, 0);
	Point2f camPos{ Track(target) };
	Clamp(camPos);

	// Draw background
	glTranslatef(-camPos.x * 0.1f, -camPos.y * 0.1f, 0);
	bg->Draw();
	glTranslatef(camPos.x * 0.1f, camPos.y * 0.1f, 0);

	// Draw middleground
	glTranslatef(-camPos.x * 0.2f, -camPos.y * 0.1f, 0);
	mg->Draw();
	glTranslatef(camPos.x * 0.2f, camPos.y * 0.1f, 0);

	glTranslatef(-camPos.x, -camPos.y, 0);
}

Point2f Camera::Track(const Rectf& target) const
{
	Point2f camPos{ (target.left + target.width / 2) - (m_Width / 2), (target.bottom + target.height / 2) - (m_Height / 2) };
	return camPos;
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	if (bottomLeftPos.x < m_LevelBoundaries.left)
	{
		bottomLeftPos.x = 0;
	}
	if (bottomLeftPos.x + m_Width > m_LevelBoundaries.left + m_LevelBoundaries.width)
	{
		bottomLeftPos.x = m_LevelBoundaries.width - m_Width;
	}

	if (bottomLeftPos.y < m_LevelBoundaries.bottom)
	{
		bottomLeftPos.y = 0;
	}
	if (bottomLeftPos.y + m_Height > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
	{
		bottomLeftPos.y = m_LevelBoundaries.height - m_Height;
	}
}