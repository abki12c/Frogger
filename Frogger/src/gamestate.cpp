#include "gamestate.hpp"
#include "level.hpp"
#include "button.hpp"
#include <iostream>
#include "util.hpp"

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

	if (m_help_button) {
		delete m_help_button;
	}

	if (m_back_button) {
		delete m_back_button;
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
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

void GameState::updateStartScreen(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_F)) {
		graphics::setFullScreen(true);
	}

	m_start_button->update(dt);
	m_quit_button->update(dt);
	m_help_button->update(dt);
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

void GameState::updateHelpScreen(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_F)) {
		graphics::setFullScreen(true);
	}

	m_back_button->update(dt);
}

void GameState::drawStartScreen()
{
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, m_wallpaper_brush);

	m_start_button->draw();
	m_quit_button->draw();
	m_help_button->draw();
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
	graphics::drawText(CANVAS_WIDTH/2 -200, CANVAS_HEIGHT/2,80,"GAME OVER",m_txt_brush);
	graphics::drawText(CANVAS_WIDTH / 2 - 80, CANVAS_HEIGHT / 2 + 150, 40, "Score: " + std::to_string(m_score), m_txt_brush);

	m_retry_button->draw();
	m_exit_button->draw();

}

void GameState::drawHelpScreen()
{	
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, m_help_screen_brush);

	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 - 100, 288, 187, m_controls_img_brush);

	graphics::drawText(CANVAS_WIDTH / 2 - 230, CANVAS_HEIGHT / 2  - 300, 40, "Use the WASD keys to move", m_txt_brush);

	graphics::drawText(CANVAS_WIDTH / 2 - 400, CANVAS_HEIGHT  - 360, 40, "Press F to toggle Fullscreen", m_txt_brush);

	graphics::drawText(CANVAS_WIDTH / 2 - 410, CANVAS_HEIGHT - 250, 30, "The Goal  is to reach the empty home bases as many times as possible", m_txt_brush);

	m_back_button->draw();
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
	} else if (m_status == STATUS_HELP) {
		updateHelpScreen(dt);
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
	} else if(m_status == STATUS_GAME_OVER) {
		drawGameOverScreen();
	} else {
		drawHelpScreen();
	}
	
}

void GameState::init()
{
	m_current_level = new Level("Level-1");
	m_player = new Player("Frog");

	m_wallpaper_brush.outline_opacity = 0;
	m_wallpaper_brush.texture = getFullAssetPath("wallpaper.png");

	m_help_screen_brush.outline_opacity = 0;
	SETCOLOR(m_help_screen_brush.fill_color, 0.047f, 0.671f, 0.588f);

	m_controls_img_brush.texture = getFullAssetPath("wasd.png");
	m_controls_img_brush.outline_opacity = 0;

	m_start_button = new Button(/*width*/ 220, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2, /*start_pos_y*/ CANVAS_HEIGHT - 270, /*draw_pos_x*/ CANVAS_WIDTH / 2, /*draw_pos_y*/ CANVAS_HEIGHT - 210, /*text*/ "START");
	m_help_button = new Button(/*width*/ 160, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2 + 5, /*start_pos_y*/ CANVAS_HEIGHT - 190, /*draw_pos_x*/ CANVAS_WIDTH / 2 + 5, /*draw_pos_y*/  CANVAS_HEIGHT - 130, /*text*/ "HELP");
	m_quit_button = new Button(/*width*/ 140, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2, /*start_pos_y*/ CANVAS_HEIGHT - 110, /*draw_pos_x*/ CANVAS_WIDTH / 2, /*draw_pos_y*/  CANVAS_HEIGHT - 50, /*text*/ "QUIT");
	m_retry_button = new Button(/*width*/ 250, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2 - 500, /*start_pos_y*/ CANVAS_HEIGHT / 2 + 340, /*draw_pos_x*/ CANVAS_WIDTH / 2 - 500, /*draw_pos_y*/ CANVAS_HEIGHT / 2 + 400, /*text*/ "RETRY");
	m_exit_button = new Button(/*width*/ 150, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2 + 300, /*start_pos_y*/ CANVAS_HEIGHT / 2 + 340, /*draw_pos_x*/ CANVAS_WIDTH / 2 + 300, /*draw_pos_y*/  CANVAS_HEIGHT / 2 + 400, /*text*/ "EXIT");
	m_back_button = new Button(/*width*/ 180, /*height*/ 60, /*start_pos_x*/ CANVAS_WIDTH / 2 + 300, /*start_pos_y*/ CANVAS_HEIGHT / 2 + 340, /*draw_pos_x*/ CANVAS_WIDTH / 2 + 300, /*draw_pos_y*/  CANVAS_HEIGHT / 2 + 400, /*text*/ "BACK");

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

	m_help_button->setOnClick([this]() {
		m_status = STATUS_HELP;
	});

	m_back_button->setOnClick([this]() {
		m_status = STATUS_START;
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
	m_help_button->setOnHoverEnter(playHoverSound);
	m_back_button->setOnHoverEnter(playHoverSound);


	graphics::preloadBitmaps(ASSET_PATH);
	graphics::setFont(getFullAssetPath("fonts/liberation-sans.regular.ttf"));
	graphics::playMusic(getFullAssetPath("sound/soundtrack.mp3"), 1.0f, true, 4000);

}