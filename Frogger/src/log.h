#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"
#include "box.h"

class Log : public GameObject, public Box
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
    float getSpeed() const { return m_speed; }
    Log(const std::string name, float x, float y, float speed, const float width, const float height, const std::string& sprite);
    ~Log();
protected:
    void debugDraw();
};
