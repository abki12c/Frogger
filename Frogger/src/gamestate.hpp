#pragma once
#include "config.hpp"
#include <string>
#include <sgg/graphics.h>

enum game_status { STATUS_START, STATUS_PLAYING, STATUS_GAME_OVER, STATUS_HELP};

class GameState {
private:
	static GameState* m_instance;
	class Player* m_player = nullptr;
	class Level* m_current_level = nullptr;
	class Button *m_start_button, *m_quit_button, *m_retry_button, *m_exit_button, *m_help_button, *m_back_button;
	bool m_debug_mode;
	const std::string m_asset_path = ASSET_PATH;
	game_status m_status = STATUS_START;
	unsigned int m_score = 0;

	graphics::Brush m_wallpaper_brush, m_help_screen_brush, m_controls_img_brush, m_txt_brush;

	void updateStartScreen(float dt);
	void updateLevelScreen(float dt);
	void updateGameOverScreen(float dt);
	void updateHelpScreen(float dt);
	void drawStartScreen();
	void drawLevelScreen();
	void drawGameOverScreen();
	void drawHelpScreen();
public:
	static GameState* getInstance();
	void releaseInstance();
	class Player* getPlayer() const { return m_player; }
	class Level* getLevel() const { return m_current_level; }
	void setDebugMode(bool mode) { m_debug_mode = mode; }
	bool getDebugMode() const { return m_debug_mode; }
	void setStatus(game_status status);
	unsigned int getScore() { return m_score; };
	void updateScore(unsigned int points);
	std::string getFullAssetPath(const std::string& asset) const;
	void update(float dt);
	void draw();
	void init();
	GameState();
	~GameState();
};