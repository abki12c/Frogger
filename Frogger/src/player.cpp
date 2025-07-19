#include "player.hpp"
#include "sgg/graphics.h"
#include "util.hpp"
#include "gamestate.hpp"

void Player::update(float dt)
{
	if (m_isOnLog) {
		m_pos_x += m_onLogSpeed * graphics::getDeltaTime();
	}

	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		if (!m_key_pressed[0] && m_animation_timer <= 0.0f) {
			m_key_pressed[0] = true;
			m_pos_y -= m_step;
			graphics::playSound(std::string(ASSET_PATH) + "sound/jump.wav", 0.5f, false);
			m_current_sprite =  "FroggerLeapU.png";
			m_last_direction = UP;
			m_animation_timer = m_leap_duration; // Reset the timer
		}
	} else {
		m_key_pressed[0] = false;
	}
	
	if (graphics::getKeyState(graphics::SCANCODE_S)) {
		if (!m_key_pressed[2] && m_animation_timer <= 0.0f) {
			m_key_pressed[2] = true;
			m_pos_y += m_step;
			graphics::playSound(std::string(ASSET_PATH) + "sound/jump.wav", 0.5f, false);
			m_current_sprite = "FroggerLeapD.png";
			m_last_direction = DOWN;
			m_animation_timer = m_leap_duration; // Reset the timer
		}
	} else {
		m_key_pressed[2] = false;
	}
	
	if (graphics::getKeyState(graphics::SCANCODE_A)) {
		if (!m_key_pressed[1] && m_animation_timer <= 0.0f) {
			m_key_pressed[1] = true;
			m_pos_x -= m_step;
			graphics::playSound(std::string(ASSET_PATH) + "sound/jump.wav", 0.5f, false);
			m_current_sprite = "FroggerLeapL.png";
			m_last_direction = LEFT;
			m_animation_timer = m_leap_duration; // Reset the timer
		}
	} else {
		m_key_pressed[1] = false;
	}

	if (graphics::getKeyState(graphics::SCANCODE_D)) {
		if (!m_key_pressed[3] && m_animation_timer <= 0.0f) {
			m_key_pressed[3] = true;
			m_pos_x += m_step;
			graphics::playSound(std::string(ASSET_PATH) + "sound/jump.wav", 0.5f, false);
			m_current_sprite = "FroggerLeapR.png";
			m_last_direction = RIGHT;
			m_animation_timer = m_leap_duration; // Reset the timer
		}
	} else {
		m_key_pressed[3] = false;
	}

	// Decrease the animation timer
	if (m_animation_timer > 0.0f) {
		m_animation_timer -= graphics::getDeltaTime() / 1000.0f;
	}

	// Switch back to idle sprite if the timer has run out
	if (m_animation_timer <= 0.0f) {
		switch (m_last_direction) {
			case UP:    m_current_sprite = "FroggerIdleU.png"; break;
			case DOWN:  m_current_sprite = "FroggerIdleD.png"; break;
			case LEFT:  m_current_sprite = "FroggerIdleL.png"; break;
			case RIGHT: m_current_sprite = "FroggerIdleR.png"; break;
		}
	}
	

	// check if player is out of bounds
	if (m_pos_x <= 0) m_pos_x = 56;
	if (m_pos_x >= CANVAS_WIDTH) m_pos_x = 1004;
	if (m_pos_y > 820) m_pos_y = 820;

	GameObject::update(dt);
}

void Player::draw()
{
	m_brush_player.texture = std::string(ASSET_PATH) + m_current_sprite;
	graphics::drawRect(m_pos_x,m_pos_y,48.f,48.f,m_brush_player);

	if (m_state->getDebugMode())
		debugDraw();
}

void Player::init()
{
	m_pos_x = CANVAS_WIDTH / 2;
	m_pos_y = CANVAS_HEIGHT - 140;

	setInitialPosX(m_pos_x);
	setInitialPosY(m_pos_y);
	
	m_brush_player.outline_opacity = 0.0f;

	m_width = 38.f;
	m_height = 30.f;

	m_current_lives = 4;
}


void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_pos_x,m_pos_y, m_width, m_height, debug_brush);

	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_pos_x - 45.f, m_pos_y - 30.f, 15.f, s, debug_brush);
}

void Player::setPosX(float x)
{
	m_pos_x = x;
}

void Player::setPosY(float y)
{
	m_pos_y = y;
}

void Player::setInitialPosX(float x)
{
	m_initialPosX = x;
}

void Player::setInitialPosY(float y)
{
	m_initialPosY = y;
}

void Player::setOnLogSpeed(float speed)
{
	m_onLogSpeed = speed;
}

void Player::setIsOnLog(bool isOnLog)
{
	m_isOnLog = isOnLog;
}

void Player::resetPlayer()
{
	m_current_sprite = "FroggerIdleU.png";
	m_last_direction = UP;
	m_animation_timer = m_leap_duration;
}

void Player::resetPlayerPosition()
{
	setPosX(getInitialPosX());
	setPosY(getInitialPosY());
	resetPlayer();
}
