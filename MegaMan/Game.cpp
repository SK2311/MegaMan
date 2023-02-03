#include "pch.h"
#include "Game.h"
#include "Avatar.h"
#include "Level.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "HUD.h"
#include "Settings.h"
#include <iostream>

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_pAvatar{new Avatar{}}
	, m_pLevel{ new Level{} }
	, m_pCamera{ new Camera{m_Window.width, m_Window.height, 2.f} }
	, m_pBulletManager{ new BulletManager{} }
	, m_Settings{ new Settings{} }
	, m_Music{"resources/Sound/MegaMan-TheWilyWars-Bomb Man.wav"}
	, m_IsMuted{false}
	, m_IsMenuOpen{false}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pEnemyManager = new EnemyManager{ m_pBulletManager, m_pAvatar, m_pLevel };

	m_pHUD = new HUD{ Point2f{ 10.f, m_Window.height - 10.f}, Point2f{430.f, m_Window.height - 10.f} };

	m_pCamera->SetLevelBoundaries(m_pLevel->GetBoundaries());

	m_pEnemyManager->CreateEnemies();

	m_Music.Play(true);
}

void Game::Cleanup( )
{
	delete m_pAvatar;
	m_pAvatar = nullptr;

	delete m_pLevel;
	m_pLevel = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pEnemyManager;
	m_pEnemyManager = nullptr;

	delete m_pBulletManager;
	m_pBulletManager = nullptr;

	delete m_pHUD;
	m_pHUD = nullptr;

	delete m_Settings;
	m_Settings = nullptr;
}

void Game::Update( float elapsedSec )
{
	if (!m_IsMenuOpen)
	{
		m_pAvatar->Update(elapsedSec, m_pLevel, m_pBulletManager, m_pEnemyManager, m_IsMuted);

		m_pEnemyManager->UpdateEnemies(elapsedSec, m_IsMuted);

		// Update bullets shot by the enemy objects
		m_pBulletManager->UpdateBullets(elapsedSec, m_pAvatar->GetShape(), m_IsMuted);

		m_pHUD->Update(m_pAvatar->GetHealth(), m_pAvatar->GetScore());
	}

	if (m_IsMuted)
	{
		m_Music.Pause();
	}
	else
	{
		m_Music.Resume();
	}
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();

	// Camera
	m_pCamera->Translate(m_pAvatar->GetShape(), m_pLevel->GetBackground(), m_pLevel->GetMiddleground());

	// Draw level
	m_pLevel->DrawLevel();

	// Draw avatar
	m_pAvatar->Draw();

	// Draw enemies
	m_pEnemyManager->DrawEnemies();

	// Draw bullets
	m_pBulletManager->DrawBullets();

	glPopMatrix();

	// Draw HUD
	m_pHUD->Draw();

	if (m_IsMenuOpen)
	{
		m_Settings->Draw(m_IsMuted);
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_i:
		std::cout << "Game controls:\n";
		std::cout << "Movement left and right: A-D\n";
		std::cout << "Movement up (jumping and going up ladders): W\n";
		std::cout << "Movement down the ladders: S\n";
		std::cout << "Shooting: Left mouse button\n";
		std::cout << "Open menu: P\n";
		break;
	case SDLK_p:
		m_IsMenuOpen = true;
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		if (!m_IsMenuOpen)
		{
			m_pAvatar->SetShooting(true);
		}
		else
		{
			Point2f mousePos{ float(e.x), float(e.y) };
			m_IsMenuOpen = m_Settings->CloseSettingsMenu(mousePos);

			if (!m_IsMuted)
			{
				m_IsMuted = m_Settings->MuteSound(mousePos);
			}
			else
			{
				m_IsMuted = !m_Settings->MuteSound(mousePos);
			}
			
		}
		break;
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		if (!m_IsMenuOpen)
		{
			m_pAvatar->SetShooting(false);
		}
		
		break;
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}