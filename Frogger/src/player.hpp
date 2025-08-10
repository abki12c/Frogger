#pragma once
#include "gameobject.hpp"
#include "config.hpp"
#include "box.h"
#include "sgg/graphics.h"

enum Direction { UP, DOWN, LEFT, RIGHT };

struct SpriteSet {
	std::string idle;
	std::string leap;
};


class Player : public GameObject, public Box
{
private:
	graphics::Brush m_brush_player;
	static const int m_max_lives = 4;
	float m_initialPosX, m_initialPosY, m_onLogSpeed;
	bool m_isOnLog = false;
	const float m_step = 79.0f;
	int m_current_lives;
	float m_animation_timer = 0.0f; // Timer to control animation
	const float m_leap_duration = 0.25f; // How long the leap sprite should be displayed (in seconds)
	Direction m_last_direction = UP; // default direction
	bool m_key_pressed[4]; // W, A, S, D keys
	std::unordered_map<Direction, SpriteSet> m_sprites;
	std::string m_current_sprite;
public:
	Player(const std::string name) : GameObject(name) {};
	void update(float dt) override;
	void draw() override;
	void init() override;

	// Getters
	float getPosX() const { return m_pos_x; }
	float getPosY() const { return m_pos_y; }
	float getInitialPosX() const { return m_initialPosX; }
	float getInitialPosY() const { return m_initialPosY; }
	bool getIsOnLog() const { return m_isOnLog; }
	int getLives() const { return m_current_lives; }

	// Seters
	void setPosX(float x);
	void setPosY(float y);
	void setInitialPosX(float x);
	void setInitialPosY(float y);
	void setOnLogSpeed(float speed);
	void setIsOnLog(bool isOnLog);

	void reduceLives() { m_current_lives -= 1; };
	void resetLives() { m_current_lives = m_max_lives; };
	void resetPlayer();
	void resetKeysPressed();
protected:
	void debugDraw();
};