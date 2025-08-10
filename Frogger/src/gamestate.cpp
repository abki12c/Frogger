#include "gamestate.hpp"
#include "sgg/graphics.h"
#include "level.hpp"
#include "util.hpp"
#include <iostream>

GameState* GameState::m_instance = nullptr;

GameState::GameState()
{
}

GameState::~GameState()
{
	if (m_player) {
		delete m_player;
	}

	if (m_current_level) {
		delete m_current_level;
	}

}

GameState* GameState::getInstance()
{
	if (m_instance == nullptr) {
		m_instance = new GameState();
	}
	return m_instance;
}

void GameState::releaseInstance()
{
	if (m_instance) delete m_instance;
}

void GameState::updateStartScreen(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_F)) {
		graphics::setFullScreen(true);
	}

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	float mouse_pos_x = graphics::windowToCanvasX(ms.cur_pos_x);
	float mouse_pos_y = graphics::windowToCanvasY(ms.cur_pos_y);

	// update START text
	if (mouse_pos_x >= CANVAS_WIDTH / 2 && mouse_pos_x <= CANVAS_WIDTH / 2 + 200 && mouse_pos_y <= CANVAS_HEIGHT - 170 && mouse_pos_y >= CANVAS_HEIGHT - 170 - 60) {
		if (!m_isInStart) {
			graphics::playSound(std::string(ASSET_PATH) + "sound/menu-change.mp3", 0.2, false);
			m_isInStart = true;
		}
		if (ms.button_left_pressed) {
			m_status = STATUS_PLAYING;
			graphics::playMusic(std::string(ASSET_PATH) + "sound/main-song.mp3", 0.5f, true, 4000);
		}
	}
	else {
		m_isInStart = false;
	}

	// update QUIT text
	if (mouse_pos_x >= CANVAS_WIDTH / 2 && mouse_pos_x <= CANVAS_WIDTH / 2 + 130 && mouse_pos_y <= CANVAS_HEIGHT - 90 && mouse_pos_y >= CANVAS_HEIGHT - 90 - 60) {
		if (!m_isInQuit) {
			graphics::playSound(std::string(ASSET_PATH) + "sound/menu-change.mp3", 0.2, false);
			m_isInQuit = true;
		}
		if (ms.button_left_pressed) {
			graphics::destroyWindow();
		}
	}
	else {
		m_isInQuit = false;
	}
	

}

void GameState::updateLevelScreen(float dt)
{
	if (!m_current_level) {
		return;
	}
	m_current_level->update(dt);
}

void GameState::updateGameOverScreen(float dt)
{
	graphics::MouseState ms;
	graphics::getMouseState(ms);


	float mouse_pos_x = graphics::windowToCanvasX(ms.cur_pos_x);
	float mouse_pos_y = graphics::windowToCanvasY(ms.cur_pos_y);

	// update RETRY text
	if (mouse_pos_x >= CANVAS_WIDTH / 2 - 500 && mouse_pos_x <= CANVAS_WIDTH / 2 - 500 + 250 && mouse_pos_y <= CANVAS_HEIGHT / 2 + 400 && mouse_pos_y >= CANVAS_HEIGHT / 2 + 400 - 60) {
		if (!m_isInRetry) {
			graphics::playSound(std::string(ASSET_PATH) + "sound/menu-change.mp3", 0.2, false);
			m_isInRetry = true;
		}
		if (ms.button_left_pressed) {
			getPlayer()->resetLives();
			getLevel()->resetLevel();
			m_score = 0;
			m_status = STATUS_PLAYING;
			graphics::playMusic(std::string(ASSET_PATH) + "sound/main-song.mp3", 0.5f, true, 4000);
		}
	}
	else {
		m_isInRetry = false;
	}

	// update EXIT text
	if (mouse_pos_x >= CANVAS_WIDTH / 2 + 300 && mouse_pos_x <= CANVAS_WIDTH / 2 + 300 + 150 && mouse_pos_y <= CANVAS_HEIGHT / 2 + 400 && mouse_pos_y >= CANVAS_HEIGHT / 2 + 400 - 60) {
		if (!m_isInExit) {
			graphics::playSound(std::string(ASSET_PATH) + "sound/menu-change.mp3", 0.2, false);
			m_isInExit = true;
		}
		if (ms.button_left_pressed) {
			graphics::destroyWindow();
		}
	}
	else {
		m_isInExit = false;
	}
}

void GameState::drawStartScreen()
{
	graphics::Brush br;
	graphics::Brush br2;
	br2.outline_opacity = 0;
	br2.texture = getFullAssetPath("wallpaper.png");
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br2);

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	
	float mouse_pos_x = graphics::windowToCanvasX(ms.cur_pos_x);
	float mouse_pos_y = graphics::windowToCanvasY(ms.cur_pos_y);

	// draw START text
	if (mouse_pos_x >= CANVAS_WIDTH / 2 && mouse_pos_x <=CANVAS_WIDTH / 2 + 200 && mouse_pos_y <= CANVAS_HEIGHT - 170 && mouse_pos_y >= CANVAS_HEIGHT - 170 -60) {
		SETCOLOR(br.fill_color, 0, 0, 255);
	}
	else {
		SETCOLOR(br.fill_color, 0, 128, 0);
	}
	graphics::drawText(CANVAS_WIDTH / 2, CANVAS_HEIGHT - 170, 60, "START", br);

	
	// draw QUIT text
	if (mouse_pos_x >= CANVAS_WIDTH / 2 && mouse_pos_x <= CANVAS_WIDTH / 2 + 130 && mouse_pos_y <= CANVAS_HEIGHT - 90 && mouse_pos_y >= CANVAS_HEIGHT - 90 - 60) {
		SETCOLOR(br.fill_color, 0, 0, 255);
	}
	else {
		SETCOLOR(br.fill_color, 0, 128, 0);
	}
	graphics::drawText(CANVAS_WIDTH / 2, CANVAS_HEIGHT - 100, 60, "QUIT", br);
}

void GameState::drawLevelScreen()
{
	if (!m_current_level) {
		return;
	}
	m_current_level->draw();
}

void GameState::drawGameOverScreen()
{
	graphics::Brush br;
	graphics::drawText(CANVAS_WIDTH/2 -200, CANVAS_HEIGHT/2,80,"GAME OVER",br);
	graphics::drawText(CANVAS_WIDTH / 2 - 80, CANVAS_HEIGHT / 2 + 150, 40, "Score: " + std::to_string(m_score), br);
	
	graphics::MouseState ms;
	graphics::getMouseState(ms);


	float mouse_pos_x = graphics::windowToCanvasX(ms.cur_pos_x);
	float mouse_pos_y = graphics::windowToCanvasY(ms.cur_pos_y);

	// draw RETRY text
	if (mouse_pos_x >= CANVAS_WIDTH / 2 -500 && mouse_pos_x <= CANVAS_WIDTH / 2 -500 + 250 && mouse_pos_y <= CANVAS_HEIGHT / 2 + 400 && mouse_pos_y >= CANVAS_HEIGHT / 2 + 400 - 60) {
		SETCOLOR(br.fill_color, 0, 0, 255);
	}
	else {
		SETCOLOR(br.fill_color, 0, 128, 0);
	}
	graphics::drawText(CANVAS_WIDTH / 2 - 500, CANVAS_HEIGHT / 2 + 400, 80, "RETRY", br);


	// draw EXIT text
	if (mouse_pos_x >= CANVAS_WIDTH / 2 +300 && mouse_pos_x <= CANVAS_WIDTH / 2 + 300 + 150 && mouse_pos_y <= CANVAS_HEIGHT / 2 + 400 && mouse_pos_y >= CANVAS_HEIGHT / 2 + 400 - 60) {
		SETCOLOR(br.fill_color, 0, 0, 255);
	}
	else {
		SETCOLOR(br.fill_color, 0, 128, 0);
	}
	graphics::drawText(CANVAS_WIDTH / 2 + 300, CANVAS_HEIGHT / 2 + 400, 80, "EXIT", br);

}

void GameState::setStatus(status_t status)
{
	m_status = status;
}

void GameState::updateScore(unsigned int points)
{
	m_score += points;
}

std::string GameState::getFullAssetPath(const std::string& asset) const
{
	return m_asset_path + asset;
}

void GameState::update(float dt)
{
	// Skip an update if a long delay is detected 
	// to avoid messing up the collision simulation
	if (dt > 500) {
		return;
	}

	if (m_status == STATUS_START) {
		updateStartScreen(dt);
	} else if (m_status == STATUS_PLAYING) {
		updateLevelScreen(dt);
	} else {
		updateGameOverScreen(dt);
	}
}

void GameState::draw()
{

	if (m_status == STATUS_START) {
		drawStartScreen();
	}
	else if (m_status == STATUS_PLAYING) {
		drawLevelScreen();
	}
	else {
		drawGameOverScreen();
	}
	
}

void GameState::init()
{
	m_current_level = new Level("Level-1");
	m_current_level->init();

	m_player = new Player("Frog");
	m_player->init();

	graphics::preloadBitmaps(ASSET_PATH);
	graphics::setFont(getFullAssetPath("fonts/liberation-sans.regular.ttf"));
	graphics::playMusic(getFullAssetPath("sound/soundtrack.mp3"), 1.0f, true, 4000);

}
