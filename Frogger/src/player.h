#pragma once
#include "gameobject.h"
#include "config.h"
#include "box.h"
#include "sgg/graphics.h"

class Player : public GameObject, public Box
{
private:
	graphics::Brush m_brush_player;
	
	float m_initialPosX, m_initialPosY, m_onLogSpeed;
	const float m_step = 79.0f;
	int m_current_lives;
	bool m_key_pressed[4] = { false, false, false, false }; // W, S, A, D keys
	enum Direction { UP, DOWN, LEFT, RIGHT };
	std::string m_current_sprite = "FroggerIdleU.png";
	float m_animation_timer = 0.0f; // Timer to control animation
	const float m_leap_duration = 0.25f; // How long the leap sprite should be displayed (in seconds)
	Direction m_last_direction = UP; // default direction
	bool m_isOnLog = false;
public:
	Player(const std::string name) : GameObject(name) {};
	void update(float dt) override;
	void draw() override;
	void init() override;
	float getPosX() const { return m_pos_x; }
	float getPosY() const { return m_pos_y; }
	float getInitialPosX() const { return m_initialPosX; }
	float getInitialPosY() const { return m_initialPosY; }
	bool getIsOnLog() const { return m_isOnLog; }
	int getLives() const { return m_current_lives; }
	void setPosX(float x);
	void setPosY(float y);
	void setInitialPosX(float x);
	void setInitialPosY(float y);
	void setOnLogSpeed(float speed);
	void setIsOnLog(bool isOnLog);
	void reduceLives() { m_current_lives -= 1; };
	void resetLives() { m_current_lives = 4; };
	void resetPlayer();
	void resetPlayerPosition();
	
protected:
	void debugDraw();
};