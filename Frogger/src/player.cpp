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

	Direction pressedDirection;
	bool keyPressed = false;

	if (graphics::getKeyState(graphics::SCANCODE_W)) { pressedDirection = UP; keyPressed = true; }
	else if (graphics::getKeyState(graphics::SCANCODE_S)) { pressedDirection = DOWN; keyPressed = true; }
	else if (graphics::getKeyState(graphics::SCANCODE_A)) { pressedDirection = LEFT; keyPressed = true; }
	else if (graphics::getKeyState(graphics::SCANCODE_D)) { pressedDirection = RIGHT; keyPressed = true; }

	
	if (keyPressed && !m_key_pressed[pressedDirection] && m_animation_timer <= 0.0f) {
		m_key_pressed[pressedDirection] = true;

		// Move player based on direction
		switch (pressedDirection) {
			case UP:    m_pos_y -= m_step; break;
			case DOWN:  m_pos_y += m_step; break;
			case LEFT:  m_pos_x -= m_step; break;
			case RIGHT: m_pos_x += m_step; break;
		}

		graphics::playSound(m_state->getFullAssetPath("sound/jump.wav"), 0.5f, false);
		m_current_sprite = m_sprites[pressedDirection].leap;
		m_last_direction = pressedDirection;
		m_animation_timer = m_leap_duration;
	}
	
	// Reset keys when no key is held
	if (!keyPressed) {
		resetKeysPressed();
	}

	if (m_animation_timer > 0.0f) {
		m_animation_timer -= dt / 1000.0f;
	} else {
		m_current_sprite = m_sprites[m_last_direction].idle;
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


void Player::resetKeysPressed() {
	for (bool& keyPressed : m_key_pressed) {
		keyPressed = false;
	}
}


Player::Player(const std::string name) : GameObject(name) {
	init();
}