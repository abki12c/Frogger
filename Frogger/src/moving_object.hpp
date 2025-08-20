#pragma once
#include "gameobject.hpp"
#include "box.h"
#include <sgg/graphics.h>

enum class ObjectType {
	vehicle, log
};

class MovingObject : public GameObject, public Box {
protected:
	ObjectType m_type;
	float m_speed;
	std::string m_sprite;
	graphics::Brush m_brush;
	virtual void debugDraw() = 0;
public:
	virtual void update(float dt) override;
	virtual void init() override;
	virtual void draw() override;

	// Getters
	float getPosX() const { return m_pos_x; }
	float getPosY() const { return m_pos_y; }
	float getWidth() const { return m_width; }
	float getHeight() const { return m_height; }
	float getSpeed() const { return m_speed; }
	ObjectType getType() const { return m_type; }

	MovingObject(const std::string& name, float x, float y, float speed, const float width, const float height, const std::string& sprite, ObjectType type);
};