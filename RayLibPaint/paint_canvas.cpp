#include "paint_canvas.h"

const std::array<Color, 7>& paint_canvas::get_colors() const
{
    return colors_;
}

const std::array<int, 6>& paint_canvas::get_brush_sizes() const
{
    return brush_sizes_;
}

void paint_canvas::draw_circle_on_canvas(const int x, const int y, const Color color, const float radius)
{
    circle_positions_.push_back({
        .x = x,
        .y = y,
        .color = color,
        .radius =  radius});
}

void paint_canvas::clear_canvas()
{
    circle_positions_.clear();
}

const std::vector<paint_canvas::circle_positions>& paint_canvas::get_circle_positions() const
{
    return circle_positions_;
}
