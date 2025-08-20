#include "gamestate.hpp"
#include "sgg/graphics.h"
#include "level.hpp"
#include "button.hpp"
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

	if (m_start_button) {
		delete m_start_button;
	}

	if (m_quit_button) {
		delete m_quit_button;
	}

	if (m_retry_button) {
		delete m_retry_button;
	}

	if (m_exit_button) {
		delete m_exit_button;
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

	m_start_button->update(dt);
	m_quit_button->update(dt);
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
	m_retry_button->update(dt);
	m_exit_button->update(dt);
}

void GameState::drawStartScreen()
{
	graphics::Brush br;
	graphics::Brush br2;
	br2.outline_opacity = 0;
	br2.texture = getFullAssetPath("wallpaper.png");
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br2);

	m_start_button->draw();
	m_quit_button->draw();
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

	m_retry_button->draw();
	m_exit_button->draw();

}

void GameState::setStatus(game_status status)
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
	// Skip an update if a long delay is detected to avoid messing up the collision simulation
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
	} else if (m_status == STATUS_PLAYING) {
		drawLevelScreen();
	} else {
		drawGameOverScreen();
	}
	
}

void GameState::init()
{
	m_current_level = new Level("Level-1");
	m_player = new Player("Frog");

	m_start_button = new Button(/*width*/ 200, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2, /*start_pos_y*/ CANVAS_HEIGHT - 230, /*draw_pos_x*/ CANVAS_WIDTH / 2, /*draw_pos_y*/ CANVAS_HEIGHT - 170, /*text*/ "START");
	m_quit_button = new Button(/*width*/ 130, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2, /*start_pos_y*/ CANVAS_HEIGHT - 150, /*draw_pos_x*/ CANVAS_WIDTH / 2, /*draw_pos_y*/  CANVAS_HEIGHT - 90, /*text*/ "QUIT");
	m_retry_button = new Button(/*width*/ 250, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2 - 500, /*start_pos_y*/ CANVAS_HEIGHT / 2 + 340, /*draw_pos_x*/ CANVAS_WIDTH / 2 - 500, /*draw_pos_y*/ CANVAS_HEIGHT / 2 + 400, /*text*/ "RETRY");
	m_exit_button = new Button(/*width*/ 150, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2 + 300, /*start_pos_y*/ CANVAS_HEIGHT / 2 + 340, /*draw_pos_x*/ CANVAS_WIDTH / 2 + 300, /*draw_pos_y*/  CANVAS_HEIGHT / 2 + 400, /*text*/ "EXIT");

	// Set click callbacks:
	m_start_button->setOnClick([this]() {
		m_status = STATUS_PLAYING;
		graphics::playMusic(getFullAssetPath("sound/main-song.mp3"), 0.5f, true, 4000);
	});

	m_quit_button->setOnClick([]() {
		graphics::destroyWindow();
	});

	m_retry_button->setOnClick([this]() {
		getPlayer()->resetLives();
		getLevel()->resetLevel();
		m_score = 0;
		m_status = STATUS_PLAYING;
		graphics::playMusic(getFullAssetPath("sound/main-song.mp3"), 0.5f, true, 4000);
	});

	m_exit_button->setOnClick([]() {
		graphics::destroyWindow();
	});

	// Play sound on hover enter
	auto playHoverSound = [this]() {
		graphics::playSound(getFullAssetPath("sound/menu-change.mp3"), 0.2f, false);
	};

	// Set hover callbacks:
	m_start_button->setOnHoverEnter(playHoverSound);
	m_quit_button->setOnHoverEnter(playHoverSound);
	m_retry_button->setOnHoverEnter(playHoverSound);
	m_exit_button->setOnHoverEnter(playHoverSound);


	graphics::preloadBitmaps(ASSET_PATH);
	graphics::setFont(getFullAssetPath("fonts/liberation-sans.regular.ttf"));
	graphics::playMusic(getFullAssetPath("sound/soundtrack.mp3"), 1.0f, true, 4000);

}