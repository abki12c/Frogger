#include "vehicle.hpp"
#include "sgg/graphics.h"
#include "config.hpp"
#include "util.hpp"
#include "gamestate.hpp"

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

Vehicle::Vehicle(const std::string& name, float x, float y, float speed, const float width, const float height, const std::string& sprite)
	: MovingObject(name, x, y, speed, width, height, sprite) {};


Vehicle::~Vehicle()
{
}
