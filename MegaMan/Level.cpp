#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include <iostream>
#include "SVGParser.h"

Level::Level()
	: m_pLevelTexture{ new Texture{"resources/Images/Level.png"} }
	, m_pBackGround{ new Texture{"resources/Images/BG.png"} }
	, m_pMiddleGround{ new Texture{"resources/Images/MG.png"} }
	, m_SVGPath{"resources/Images/Level.svg"}
	, m_LadderSVGPath{"resources/Images/Level_Ladder.svg"}
	, m_Boundaries{0,0,m_pLevelTexture->GetWidth(), m_pLevelTexture->GetHeight()}
	, m_Spikes{SetSpikes()}
{
	SVGParser::GetVerticesFromSvgFile(m_SVGPath, m_Vertices);
	SVGParser::GetVerticesFromSvgFile(m_LadderSVGPath, m_VerticesLadder);
}

Level::~Level()
{
	delete m_pLevelTexture;
	m_pLevelTexture = nullptr;

	delete m_pBackGround;
	m_pBackGround = nullptr;

	delete m_pMiddleGround;
	m_pMiddleGround = nullptr;
}

void Level::DrawLevel() const
{
	//m_pBackGround->Draw();

	m_pLevelTexture->Draw();
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, float ladderY, bool isClimbingDown) const
{
	Point2f topPoint{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height + 1 };
	Point2f bottomPoint{ actorShape.left + actorShape.width / 2, actorShape.bottom };
	Point2f centrePoint{ actorShape.left + (actorShape.width / 2), actorShape.bottom + (actorShape.height / 2) };
	Point2f topLeftPoint{ (actorShape.left - 1), (actorShape.bottom + actorShape.height) };
	Point2f bottomLeftPoint{ (actorShape.left - 1), actorShape.bottom + 1 };
	Point2f topRightPoint{ (actorShape.left + actorShape.width) + 1, (actorShape.bottom + actorShape.height) };
	Point2f bottomRightPoint{ (actorShape.left + actorShape.width) + 1, actorShape.bottom + 1 };
	utils::HitInfo hitInfo{};

	int verticesSize{ int(m_Vertices.size()) };
	for (int i{}; i < verticesSize; ++i)
	{
		// Check for bottom raycast
		if (utils::Raycast(m_Vertices[i], topPoint, bottomPoint, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.y = 0.f;
		}
		// Check for right raycast
		if (utils::Raycast(m_Vertices[i], centrePoint, topRightPoint, hitInfo) || utils::Raycast(m_Vertices[i], centrePoint, bottomRightPoint, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x - actorShape.width - 1;
			actorVelocity.x = 0.f;
		}
		// Check for left raycast
		if (utils::Raycast(m_Vertices[i], centrePoint, topLeftPoint, hitInfo) || utils::Raycast(m_Vertices[i], centrePoint, bottomLeftPoint, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x + 1;
			actorVelocity.x = 0.f;
		}
		// Check for top raycast
		if (utils::Raycast(m_Vertices[i], centrePoint, topPoint, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height - 1;
			actorVelocity.x = 0.f;
			actorVelocity.y = 0.f;
		}
	}

	for (int i{}; i < m_VerticesLadder.size(); ++i)
	{
		if (actorShape.bottom <= GetYTopOfLadder(actorShape) && !isClimbingDown)
		{
			if (utils::Raycast(m_VerticesLadder[i], Point2f{ bottomPoint.x, actorShape.bottom + 1 }, bottomPoint, hitInfo))
			{
				//std::cout << "climbing\n";
				actorShape.bottom = hitInfo.intersectPoint.y;
				actorVelocity.x = 0.f;
				actorVelocity.y = 0.f;
			}
		}
	}	
}

bool Level::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity, float ladderY) const
{
	Point2f topPoint{ actorShape.left + actorShape.width / 2, actorShape.bottom + 1.f };
	Point2f bottomPoint{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1.f };
	utils::HitInfo hitInfo{};

	bool raycastHit{ false };
	for (int i{}; i < m_Vertices.size(); ++i)
	{
		raycastHit = utils::Raycast(m_Vertices[i], topPoint, bottomPoint, hitInfo);
		if (raycastHit)
		{
			break;
		}
	}

	for (int i{}; i < m_VerticesLadder.size(); ++i)
	{
		if (!raycastHit)
		{
			raycastHit = utils::Raycast(m_VerticesLadder[i], topPoint, bottomPoint, hitInfo);
		}

		if (raycastHit)
		{
			break;
		}
	}
	return raycastHit;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

bool Level::IsOverlappingWithLadder(const Rectf& actorShape) const
{
	Point2f playerPos{ actorShape.left + actorShape.width / 2, actorShape.bottom };
	bool isOverlapping{ false };
	for (int i{}; i < m_VerticesLadder.size(); ++i)
	{
		isOverlapping = utils::IsPointInPolygon(playerPos, m_VerticesLadder[i]);
		if (isOverlapping)
		{
			break;
		}
	}
	return isOverlapping;
}

float Level::GetYTopOfLadder(const Rectf& actorShape) const
{
	Point2f playerPos{ actorShape.left + actorShape.width / 2, actorShape.bottom };
	Point2f ladderYPos{};
	bool isOverlapping{ false };
	for (int i{}; i < m_VerticesLadder.size(); ++i)
	{
		isOverlapping = utils::IsPointInPolygon(playerPos, m_VerticesLadder[i]);
		if (isOverlapping)
		{
			ladderYPos = m_VerticesLadder[i][1];
			break;
		}
	}
	return ladderYPos.y;
}

Point2f Level::GetXTopOfLadder(const Rectf& actorShape) const
{
	Point2f playerPos{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1 };
	Point2f ladderXPos{};
	bool isOverlapping{ false };
	for (int i{}; i < m_VerticesLadder.size(); ++i)
	{
		// check for overlapping ladder
		isOverlapping = utils::IsPointInPolygon(playerPos, m_VerticesLadder[i]);
		if (isOverlapping)
		{
			// get the top x-positions of the ladder
			ladderXPos.x = m_VerticesLadder[i][1].x;
			ladderXPos.y = m_VerticesLadder[i][2].x;
			break;
		}
	}
	return ladderXPos;
}

bool Level::SpikesHitDetection(const Rectf& player) const
{
	bool result{ false };
	for (int i{}; i < m_Spikes.size(); ++i)
	{
		result = utils::IsOverlapping(m_Spikes[i], player);
		if (result)
		{
			break;
		}
	}
	return result;
}

Texture* Level::GetBackground() const
{
	return m_pBackGround;
}

Texture* Level::GetMiddleground() const
{
	return m_pMiddleGround;
}

std::vector<Rectf> Level::SetSpikes() const
{
	std::vector<Rectf> spikes;
	spikes.push_back(Rectf(1537.f, 257.f, 128.f, 16.f));
	spikes.push_back(Rectf(2817.f, 737.f, 96.f, 16.f));
	spikes.push_back(Rectf(2897.f, 977.f, 176.f, 16.f));
	return spikes;
}