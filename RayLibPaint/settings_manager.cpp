#include "settings_manager.h"

const std::array<Color, 7>& settings_manager::get_colors() const
{
    return colors_;
}

void settings_manager::draw_circle_on_canvas(const int x, const int y, const Color color, const float radius)
{
    circle_positions_.push_back({
        .x = x,
        .y = y,
        .color = color,
        .radius =  radius});
}

void settings_manager::clear_canvas()
{
    circle_positions_.clear();
}

const std::vector<settings_manager::circle_positions>& settings_manager::get_circle_positions() const
{
    return circle_positions_;
}