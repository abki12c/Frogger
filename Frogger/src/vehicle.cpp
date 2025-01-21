#include "vehicle.h"
#include "sgg/graphics.h"
#include "config.h"
#include "gamestate.h"
#include "util.h"

void Vehicle::update(float dt)
{
	m_pos_x += m_speed * graphics::getDeltaTime();

	if (m_speed > 0 && m_pos_x > CANVAS_WIDTH + m_width || m_speed < 0 && m_pos_x < -m_width) {
		m_active = false;
	}
}

void Vehicle::init()
{
	m_brush.outline_opacity = 0.0f;
	m_brush.texture = std::string(ASSET_PATH) + m_sprite;

	if (m_speed > 0) {
		m_pos_x -= m_width;
	} else {
		m_pos_x += m_width;
	}
}
	

void Vehicle::draw()
{
	
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush);

	if (m_state->getDebugMode()) {
		debugDraw();
	}
	
}

void Vehicle::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 0, 255, 255);
	SETCOLOR(debug_brush.outline_color, 137, 207, 240);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, debug_brush);

	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 0, 255, 255);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_pos_x - 45.f, m_pos_y - 30.f, 15.f, s, debug_brush);
}

Vehicle::Vehicle(const std::string name,float x, float y, float speed, const float width, const float height, const std::string& sprite)
	: GameObject(name), m_pos_x(x), m_pos_y(y), m_speed(speed), m_width(width), m_height(height), m_sprite(sprite) {
	init();
}


Vehicle::~Vehicle()
{
}
