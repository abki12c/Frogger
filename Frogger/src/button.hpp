#pragma once
#include <sgg/graphics.h>

class Button
{
private:
	graphics::MouseState m_mouse_state;
	float m_width, m_height, m_start_pos_x, m_start_pos_y, m_draw_pos_x, m_draw_pos_y;
	bool m_wasHovered = false;
	bool m_wasClicked = false;
	const std::string m_text;

	std::function<void()> onClick;
	std::function<void()> onHoverEnter;
	std::function<void()> onHoverExit;
public:
	Button(float width, float height, float start_pox_x, float start_pos_y, float draw_pos_x, float draw_pos_y, const std::string& text);
	bool isHovered();
	bool isClicked();

	void setOnClick(std::function<void()> callback) { onClick = callback; }
	void setOnHoverEnter(std::function<void()> callback) { onHoverEnter = callback; }
	void setOnHoverExit(std::function<void()> callback) { onHoverExit = callback; }

	void update(float dt);
	void draw();
};