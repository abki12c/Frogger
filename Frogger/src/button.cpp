#include "button.hpp"
#include <algorithm>
#include "util.hpp"

Button::Button(float width, float height, float start_pos_x, float start_pos_y, float draw_pos_x, float draw_pos_y, const std::string& text)
    : m_width(width), m_height(height), m_start_pos_x(start_pos_x), m_start_pos_y(start_pos_y), m_draw_pos_x(draw_pos_x), m_draw_pos_y(draw_pos_y), m_text(text)
{
	graphics::getMouseState(m_mouse_state);
}

bool Button::isHovered()
{
	float mouse_pos_x = graphics::windowToCanvasX(m_mouse_state.cur_pos_x);
	float mouse_pos_y = graphics::windowToCanvasY(m_mouse_state.cur_pos_y);
	return (mouse_pos_x >= m_start_pos_x && mouse_pos_x <= m_start_pos_x + m_width) &&
		(mouse_pos_y >= m_start_pos_y && mouse_pos_y <= m_start_pos_y + m_height);
}

bool Button::isClicked()
{
	return m_mouse_state.button_left_pressed;
}

void Button::update(float dt)
{
    graphics::getMouseState(m_mouse_state);
    bool hovered = isHovered();

    // Hover enter event
    if (hovered && !m_wasHovered && onHoverEnter) {
        onHoverEnter();
    }
    // Hover exit event
    if (!hovered && m_wasHovered && onHoverExit) {
        onHoverExit();
    }

    m_wasHovered = hovered;
    bool clickedNow = hovered && isClicked();

    // Click event
    if (clickedNow && !m_wasClicked && onClick) {
        onClick();
    }
    m_wasClicked = clickedNow;
}

void Button::draw()
{
    graphics::Brush br;
    // Background color
    if (isHovered()) {
        SETCOLOR(br.fill_color, 0, 0, 255);  // Blue on hover
    } else {
        SETCOLOR(br.fill_color, 0, 128, 0);  // Green normally
    }
    
    graphics::drawText(m_draw_pos_x, m_draw_pos_y, 70, m_text, br);
}