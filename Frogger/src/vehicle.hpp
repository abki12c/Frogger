#pragma once
#include "gameobject.hpp"
#include "sgg/graphics.h"
#include "box.h"

class Vehicle : public GameObject, public Box
{
private:
	float m_pos_x, m_pos_y;
	float m_speed;
	const float m_width, m_height;
	std::string m_sprite;
	graphics::Brush m_brush;
public:
	void update(float dt) override;
	void init() override;
	void draw() override;
	float getPosX() const { return m_pos_x; }
	float getPosY() const { return m_pos_y; }
	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; }
	Vehicle(const std::string name, float x, float y, float speed, const float width, const float height, const std::string& sprite);
	~Vehicle();
protected:
	void debugDraw();
};