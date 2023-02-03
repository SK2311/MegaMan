#include "pch.h"
#include "Avatar.h"
#include "Texture.h"
#include "Level.h"
#include "BulletManager.h"
#include "EnemyManager.h"

Avatar::Avatar()
	: m_HorSpeed{ 100.f }
	, m_VerSpeed{ 100.f }
	, m_JumpSpeed{ 350.f }
	, m_Acceleration{ 0.f, -981.f }
	, m_State{ AvatarState::moving }
	, m_pAvatarSpriteTexture{ new Texture{"resources/Images/MegaMan.png"} }
	, m_IsShooting{ false }
	, m_IsLookingLeft{ false }
	, m_IsClimbing{ false }
	, m_TopOfLadder{ 0.f }
	, m_AnimTime{ 0.f }
	, m_AnimFrame{ 0 }
	, m_IdleWidth{ 21.f }
	, m_IdleHeight{ 29.f }
	, m_JumpWidth{ 26.f }
	, m_JumpHeight{ 36.f }
	, m_ShootIdleWidth{ 31.f }
	, m_ShootIdleHeight{ 27.f }
	, m_ShootRunWidth{ 28.f }
	, m_ShootRunHeight{ 29.f }
	, m_ClimbWidth{ 18.f }
	, m_ClimbHeight{ 32.f }
	, m_DieWidth{ 32.f }
	, m_Dieheight{ 32.f }
	, m_ShootJumpWidth{29.f}
	, m_ShootJumpHeight{36.f}
	, m_SrcRect{}
	, m_IsClimbingDown{ false }
	, m_FrameCounter{0}
	, m_ResetCounter{false}
	, m_HitCoolDown{2.f}
	, m_AccuHit{0.f}
	, m_Health{10}
	, m_TakenDamage{false}
	, m_AccuFlicker{0.f}
	, m_FlickerCoolDown{0.2f}
	, m_TotalFlickerTime{2.f}
	, m_AccuTotalFlickerTime{0.f}
	, m_IsDrawing{true}
	, m_HasPlayedDie{false}
	, m_Score{}
	, m_MegamanDamage{ "resources/Sound/07 - MegamanDamage.wav" }
	, m_MegamanDie{ "resources/Sound/08 - MegamanDefeat.wav" }
	, m_IsMuted{false}
{
	m_DstRect = Rectf(1590.f, 529.f, m_IdleWidth, m_IdleHeight);
}

Avatar::~Avatar()
{
	delete m_pAvatarSpriteTexture;
	m_pAvatarSpriteTexture = nullptr;
}

void Avatar::Update(float elapsedSec, const Level* level, BulletManager* bulletManager, EnemyManager* enemyManager, const bool isMuted)
{
	UpdateFrame(elapsedSec);
	m_IsMuted = isMuted;

	if (m_State != AvatarState::die)
	{
		StateManager(level);
		InputManager(level);
		Move(elapsedSec);
		SetTopOfLadder(level);
		level->HandleCollision(m_DstRect, m_Velocity, m_TopOfLadder, m_IsClimbingDown);
		bulletManager->CreateBullet(m_IsShooting, m_DstRect, GetDirection(), 0.f);

		std::vector<Rectf> enemyShapes;
		enemyManager->GetRects(enemyShapes);
		bulletManager->UpdateBullets(elapsedSec, enemyShapes);

		TakeDamage(elapsedSec, level, bulletManager, enemyManager);

		if (m_TakenDamage)
		{
			FlickerAvatar(elapsedSec);
		}
	}
	else
	{
		if (!m_HasPlayedDie)
		{
			if (!m_IsMuted)
			{
				m_MegamanDie.Play(0);
			}
			
			m_HasPlayedDie = true;
		}

		if (!level->IsOnGround(m_DstRect, m_Velocity, 0.f))
		{
			m_Velocity.y += m_Acceleration.y * elapsedSec;
			m_DstRect.bottom += m_Velocity.y * elapsedSec;
		}
	}
}

void Avatar::Draw() const
{
	if (m_IsDrawing)
	{
		DrawSprite();
	}
}

Rectf Avatar::GetShape() const
{
	return m_DstRect;
}

void Avatar::SetShooting(bool isShooting)
{
	m_IsShooting = isShooting;
}

bool Avatar::GetShooting() const
{
	return m_IsShooting;
}

void Avatar::StateManager(const Level* level)
{
	if (m_State != AvatarState::die)
	{
		if (m_Health <= 0 || m_DstRect.bottom + m_DstRect.height <= 0.f)
		{
			m_State = AvatarState::die;
		}
		else if (m_State != AvatarState::climbing)
		{
			if (!level->IsOnGround(m_DstRect, m_Velocity, m_TopOfLadder))
			{
				if (level->IsOverlappingWithLadder(m_DstRect) && m_IsClimbing)
				{
					m_State = AvatarState::climbing;
				}
				else
				{
					if (m_IsShooting)
					{
						m_State = AvatarState::shootingJump;
					}
					else
					{
						m_State = AvatarState::jumping;
					}
				}
			}
			else if (m_Velocity.x != 0 || m_Velocity.y != 0)
			{
				if (m_IsShooting)
				{
					m_State = AvatarState::shootingRunning;
				}
				else
				{
					m_State = AvatarState::moving;
				}
			}
			else if (level->IsOnGround(m_DstRect, m_Velocity, m_TopOfLadder))
			{
				if (m_IsShooting)
				{
					m_State = AvatarState::shootingIdle;
				}
				else
				{
					m_State = AvatarState::idle;
				}
			}
		}
		else
		{
			// TODO: check states for running on top of ladder
			if (!level->IsOverlappingWithLadder(m_DstRect))
			{
				m_IsClimbing = false;
				m_State = AvatarState::jumping;
			}
			else if (m_DstRect.bottom >= m_TopOfLadder)
			{
				m_IsClimbing = false;
				m_State = AvatarState::moving;
			}
			else if (level->IsOnGround(m_DstRect, m_Velocity, m_TopOfLadder))
			{
				m_IsClimbing = false;
				m_State = AvatarState::idle;
			}
		}
	}
}

void Avatar::InputManager(const Level* level)
{
	const Uint8* keyboardState{ SDL_GetKeyboardState(NULL) };
	if (keyboardState[SDL_SCANCODE_A])
	{
		m_Velocity.x = -m_HorSpeed;
		m_IsLookingLeft = true;
	}
	else if (keyboardState[SDL_SCANCODE_D])
	{
		m_Velocity.x = m_HorSpeed;
		m_IsLookingLeft = false;
	}
	else
	{
		m_Velocity.x = 0;
	}

	if (keyboardState[SDL_SCANCODE_W])
	{
		if (level->IsOverlappingWithLadder(m_DstRect))
		{
			// Climb
			m_IsClimbing = true;
			m_Velocity.y = m_VerSpeed;
		}
		else if (level->IsOnGround(m_DstRect, m_Velocity, m_TopOfLadder))
		{
			m_Velocity.y += m_JumpSpeed;
		}
	}
	else if (keyboardState[SDL_SCANCODE_S])
	{
		if (level->IsOverlappingWithLadder(m_DstRect))
		{
			// Climb down while on ladder
			m_IsClimbing = true;
			m_Velocity.y = -m_VerSpeed;
		}
		else if (level->IsOnGround(m_DstRect, m_Velocity, m_TopOfLadder))
		{
			// Climb down when standing on top of the ladder
			m_IsClimbingDown = true;
			m_IsClimbing = true;
			m_Velocity.y = -m_VerSpeed;
		}
	}
	else
	{
		m_IsClimbing = false;
		m_IsClimbingDown = false;
	}
}

void Avatar::Move(float elapsedSec)
{
	if (m_State != AvatarState::climbing)
	{
		// apply gravity
		m_Velocity.y += m_Acceleration.y * elapsedSec;
	}
	m_DstRect.left += m_Velocity.x * elapsedSec;
	m_DstRect.bottom += m_Velocity.y * elapsedSec;
}

void Avatar::UpdateFrame(float elapsedSec)
{
	if (m_State != AvatarState::die)
	{
		m_AnimTime += elapsedSec;
		float frameSec{ float(1.f / GetNrFrames()) };
		if (m_AnimTime > frameSec)
		{
			++m_AnimFrame %= GetNrFrames();
			m_AnimTime -= frameSec;
		}
		SetSourceRect();
	}
	else if (m_FrameCounter != 2)
	{
		if (!m_ResetCounter)
		{
			m_AnimTime = 0;
			m_AnimFrame = 0;
			m_ResetCounter = true;
		}

		m_AnimTime += elapsedSec;
		float frameSec{ float(1.f / GetNrFrames()) };
		if (m_AnimTime > frameSec)
		{
			++m_AnimFrame %= GetNrFrames();
			m_AnimTime -= frameSec;
		}
		SetSourceRect();

		if (m_AnimFrame == 2)
		{
			m_FrameCounter = m_AnimFrame;
		}
	}
}

int Avatar::GetNrFrames()
{
	switch (m_State)
	{
	case Avatar::AvatarState::idle:
		return 3;
		break;
	case Avatar::AvatarState::moving:
		return 6;
		break;
	case Avatar::AvatarState::jumping:
		return 2;
		break;
	case Avatar::AvatarState::shootingIdle:
		return 1;
		break;
	case Avatar::AvatarState::shootingRunning:
		return 6;
		break;
	case Avatar::AvatarState::climbing:
		return 4;
		break;
	case Avatar::AvatarState::die:
		return 3;
		break;
	case Avatar::AvatarState::shootingJump:
		return 1;
		break;
	default:
		return -1;
		break;
	}
}

void Avatar::SetSourceRect()
{
	int row{ int(m_State) };

	switch (m_State)
	{
	case Avatar::AvatarState::idle:
		m_SrcRect.width = m_IdleWidth;
		m_SrcRect.height = m_IdleHeight;
		m_SrcRect.left = float(m_AnimFrame % GetNrFrames()) * m_IdleWidth;
		m_SrcRect.bottom = (row + 1) * m_IdleHeight;
		break;
	case Avatar::AvatarState::moving:
		m_SrcRect.width = m_IdleWidth;
		m_SrcRect.height = m_IdleHeight;
		m_SrcRect.left = float(m_AnimFrame % GetNrFrames()) * m_IdleWidth;
		m_SrcRect.bottom = (row + 1) * m_IdleHeight;
		break;
	case Avatar::AvatarState::jumping:
		m_SrcRect.width = m_JumpWidth;
		m_SrcRect.height = m_JumpHeight;
		m_SrcRect.left = float(m_AnimFrame % GetNrFrames()) * m_JumpWidth;
		m_SrcRect.bottom = (row + 1) * m_JumpHeight;
		break;
	case Avatar::AvatarState::shootingIdle:
		m_SrcRect.width = m_ShootIdleWidth;
		m_SrcRect.height = m_ShootIdleHeight;
		m_SrcRect.left = float(m_AnimFrame % GetNrFrames()) * m_ShootIdleWidth;
		m_SrcRect.bottom = (row + 2) * m_ShootIdleHeight;
		break;
	case Avatar::AvatarState::shootingRunning:
		m_SrcRect.width = m_ShootRunWidth;
		m_SrcRect.height = m_ShootRunHeight;
		m_SrcRect.left = float(m_AnimFrame % GetNrFrames()) * m_ShootRunWidth;
		m_SrcRect.bottom = (row + 2) * m_ShootRunHeight;
		break;
	case Avatar::AvatarState::climbing:
		m_SrcRect.width = m_ClimbWidth;
		m_SrcRect.height = m_ClimbHeight;
		m_SrcRect.left = float(m_AnimFrame % GetNrFrames()) * m_ClimbWidth;
		m_SrcRect.bottom = (row + 2) * m_ClimbHeight;
		break;
	case Avatar::AvatarState::die:
		m_SrcRect.width = m_DieWidth;
		m_SrcRect.height = m_Dieheight;
		m_SrcRect.left = float(m_AnimFrame % GetNrFrames()) * m_DieWidth;
		m_SrcRect.bottom = (row + 2) * m_Dieheight;
		break;
	case Avatar::AvatarState::shootingJump:
		m_SrcRect.width = m_ShootJumpWidth;
		m_SrcRect.height = m_ShootJumpHeight;
		m_SrcRect.left = float(m_AnimFrame % GetNrFrames()) * m_ShootJumpWidth;
		m_SrcRect.bottom = (row + 2) * m_ShootJumpHeight;
		break;
	}

	m_DstRect.width = m_SrcRect.width;
	m_DstRect.height = m_SrcRect.height;
}

void Avatar::DrawSprite() const
{
	glPushMatrix();
	glTranslatef(m_DstRect.left, m_DstRect.bottom, 0);

	if (m_IsLookingLeft)
	{
		glScalef(-1, 1, 0);
		glTranslatef(-m_DstRect.width, 0, 0);
	}
	else
	{
		glScalef(1, 1, 0);
	}
	m_pAvatarSpriteTexture->Draw(Rectf{ 0,0.f,m_DstRect.width, m_DstRect.height }, m_SrcRect);
	glPopMatrix();
}

void Avatar::SetTopOfLadder(const Level* level)
{
	if (level->GetYTopOfLadder(m_DstRect) > 0.f)
	{
		m_TopOfLadder = level->GetYTopOfLadder(m_DstRect) + 1;
	}
}

float Avatar::GetDirection() const
{
	if (m_IsLookingLeft)
	{
		return -1.f;
	}
	else
	{
		return 1.f;
	}
}

int Avatar::GetHealth() const
{
	return m_Health;
}

int Avatar::GetScore() const
{
	return m_Score;
}

void Avatar::AddScore(int addValue)
{
	m_Score += addValue;
}

void Avatar::TakeDamage(float elapsedSec, const Level* level, BulletManager* bulletManager, EnemyManager* enemyManager)
{
	m_AccuHit += elapsedSec;
	if (m_AccuHit > m_HitCoolDown)
	{
		// Take damage when hit by an enemy, enemy bullet or spikes
		if (enemyManager->HitPlayer(m_DstRect) || bulletManager->HitDetection(m_DstRect) || level->SpikesHitDetection(m_DstRect))
		{
			if (!m_IsMuted)
			{
				m_MegamanDamage.Play(0);
			}

			--m_Health;
			m_AccuHit = 0.f;
			if (m_Health > 0)
			{
				m_TakenDamage = true;
			}			
		}
	}
}

void Avatar::FlickerAvatar(float elapsedSec)
{
	m_AccuFlicker += elapsedSec;
	m_AccuTotalFlickerTime += elapsedSec;

	if (m_AccuTotalFlickerTime <= m_TotalFlickerTime)
	{
		if (m_AccuFlicker >= m_FlickerCoolDown)
		{
			m_IsDrawing = !m_IsDrawing;
			m_AccuFlicker = 0.f;
		}
	}
	else
	{
		m_AccuTotalFlickerTime = 0.f;
		m_TakenDamage = false;
		m_IsDrawing = true;
	}
}