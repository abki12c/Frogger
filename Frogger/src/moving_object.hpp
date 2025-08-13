#pragma once
#include "gameobject.hpp"
#include "box.h"
#include <sgg/graphics.h>

class MovingObject : public GameObject, public Box {
protected:
	float m_pos_x, m_pos_y, m_speed;
	const float m_width, m_height;
	std::string m_sprite;
	graphics::Brush m_brush;
	void debugDraw();
public:
	virtual void update(float dt) override;
	virtual void init() override;
	virtual void draw() override;

	// Getters
	virtual float getPosX() const { return m_pos_x; }
	virtual float getPosY() const { return m_pos_y; }
	virtual float getWidth() const { return m_width; }
	virtual float getHeight() const { return m_height; }
	virtual float getSpeed() const { return m_speed;  }

	MovingObject(const std::string& name, float x, float y, float speed, const float width, const float height, const std::string& sprite);
};