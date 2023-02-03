#pragma once
#include "SoundStream.h"

class Avatar;
class Level;
class Camera;
class EnemyManager;
class BulletManager;
class HUD;
class Settings;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	Avatar* m_pAvatar;
	const Level* m_pLevel;
	Camera* m_pCamera;
	EnemyManager* m_pEnemyManager;
	BulletManager* m_pBulletManager;
	HUD* m_pHUD;
	Settings* m_Settings;

	SoundStream m_Music;

	bool m_IsMuted;
	bool m_IsMenuOpen;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
};