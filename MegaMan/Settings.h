#pragma once
class Texture;

class Settings
{
public:
	explicit Settings();
	Settings(const Settings& other) = delete;
	Settings& operator=(const Settings& other) = delete;
	Settings(Settings&& other) = delete;
	Settings& operator=(Settings&& other) = delete;
	~Settings();

	void Draw(const bool isMuted) const;
	bool CloseSettingsMenu(Point2f mousePos);
	bool MuteSound(Point2f mousePos);
private:

	Texture* m_Menu;
	Texture* m_MuteBar;
	Rectf m_CloseRect;
	Rectf m_MuteRect;
};

