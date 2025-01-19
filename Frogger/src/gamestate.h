#pragma once


class GameState {
public:
	enum status_t { STATUS_START, STATUS_PLAYING, STATUS_GAME_OVER };
private:
	static GameState* m_instance;
	class Player* m_player = nullptr;
	class Level* m_current_level = nullptr;
	bool m_isInStart, m_isInQuit,m_isInRetry,m_isInExit = false;
	bool m_debug_mode;
	status_t m_status = STATUS_START;
	unsigned int m_score = 0;

	void updateStartScreen(float dt);
	void updateLevelScreen(float dt);
	void updateGameOverScreen(float dt);
	void drawStartScreen();
	void drawLevelScreen();
	void drawGameOverScreen();
public:
	static GameState* getInstance();
	void releaseInstance();
	class Player* getPlayer() const { return m_player; }
	class Level* getLevel() const { return m_current_level; }
	void setDebugMode(bool mode) { m_debug_mode = mode; }
	bool getDebugMode() const { return m_debug_mode; }
	void setStatus(status_t status);
	unsigned int getScore() { return m_score; };
	void updateScore(unsigned int points);
	void update(float dt);
	void draw();
	void init();
	GameState();
	~GameState();
};