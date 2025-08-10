#include "player.hpp"
#include "sgg/graphics.h"
#include "util.hpp"
#include "gamestate.hpp"
#include <algorithm>

void Player::update(float dt)
{
	if (m_isOnLog) {
		m_pos_x += m_onLogSpeed * dt;
	}

	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		if (!m_key_pressed[KEY_W] && m_animation_timer <= 0.0f) {
			m_key_pressed[KEY_W] = true;
			m_pos_y -= m_step;
			graphics::playSound(m_state->getFullAssetPath("sound/jump.wav"), 0.5f, false);
			m_current_sprite = m_sprites[UP].leap;
			m_last_direction = UP;
			m_animation_timer = m_leap_duration; // Reset the timer
		}
	} else {
		m_key_pressed[KEY_W] = false;
	}
	
	if (graphics::getKeyState(graphics::SCANCODE_S)) {
		if (!m_key_pressed[KEY_S] && m_animation_timer <= 0.0f) {
			m_key_pressed[KEY_S] = true;
			m_pos_y += m_step;
			graphics::playSound(m_state->getFullAssetPath("sound/jump.wav"), 0.5f, false);
			m_current_sprite = m_sprites[DOWN].leap;
			m_last_direction = DOWN;
			m_animation_timer = m_leap_duration; // Reset the timer
		}
	} else {
		m_key_pressed[KEY_S] = false;
	}
	
	if (graphics::getKeyState(graphics::SCANCODE_A)) {
		if (!m_key_pressed[KEY_A] && m_animation_timer <= 0.0f) {
			m_key_pressed[KEY_A] = true;
			m_pos_x -= m_step;
			graphics::playSound(m_state->getFullAssetPath("sound/jump.wav"), 0.5f, false);
			m_current_sprite = m_sprites[LEFT].leap;
			m_last_direction = LEFT;
			m_animation_timer = m_leap_duration; // Reset the timer
		}
	} else {
		m_key_pressed[KEY_A] = false;
	}

	if (graphics::getKeyState(graphics::SCANCODE_D)) {
		if (!m_key_pressed[KEY_D] && m_animation_timer <= 0.0f) {
			m_key_pressed[KEY_D] = true;
			m_pos_x += m_step;
			graphics::playSound(m_state->getFullAssetPath("sound/jump.wav"), 0.5f, false);
			m_current_sprite = m_sprites[RIGHT].leap;
			m_last_direction = RIGHT;
			m_animation_timer = m_leap_duration; // Reset the timer
		}
	} else {
		m_key_pressed[KEY_D] = false;
	}

	// Decrease the animation timer
	if (m_animation_timer > 0.0f) {  
		m_animation_timer -= dt / 1000.0f;	
	}

	// Switch back to idle sprite if the timer has run out
	if (m_animation_timer <= 0.0f) {
		switch (m_last_direction) {
			case UP:    m_current_sprite = m_sprites[UP].idle; break;
			case DOWN:  m_current_sprite = m_sprites[DOWN].idle; break;
			case LEFT:  m_current_sprite = m_sprites[LEFT].idle; break;
			case RIGHT: m_current_sprite = m_sprites[RIGHT].idle; break;
		}
	}
	

	// Clamp player position to stay within bounds
	m_pos_x = std::clamp(m_pos_x, 56.0f, 1004.0f);
	m_pos_y = std::clamp(m_pos_y, 0.0f, 820.0f);
}

void Player::draw()
{
	m_brush_player.texture = m_state->getFullAssetPath(m_current_sprite);
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

	m_current_lives = m_max_lives;

	m_sprites[UP] = { "FroggerIdleU.png", "FroggerLeapU.png" };
	m_sprites[DOWN] = { "FroggerIdleD.png", "FroggerLeapD.png" };
	m_sprites[LEFT] = { "FroggerIdleL.png", "FroggerLeapL.png" };
	m_sprites[RIGHT] = { "FroggerIdleR.png", "FroggerLeapR.png" };

	m_current_sprite = m_sprites[UP].idle;
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
	setPosX(getInitialPosX());
	setPosY(getInitialPosY());

	m_current_sprite = m_sprites[UP].idle;
	m_last_direction = UP;
	m_animation_timer = m_leap_duration;
}
