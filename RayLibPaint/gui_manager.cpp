//
// Created by Nikodem Bąkowski on 15/06/2026.
//

#include "gui_manager.h"

#include <format>
#include <array>
#include <optional>
#include <tuple>

gui_manager::gui_manager(const int width) : window_width_(width)
{
    int position_x = 15;
    for (const Color color : colors_)
    {
        constexpr int size = 50;
        constexpr int position_y = 15;
        color_square_positions_.push_back({ position_x, position_y, size, color });
        position_x += 65;
    }

    color_square_bar_size_ = position_x;
}

const std::array<Color, 7>& gui_manager::get_colors() const
{
    return colors_;
}

void gui_manager::draw_circle_on_canvas(const int x, const int y, const Color color, const float radius)
{
    circle_positions_.push_back({ x, y, color, radius });
}

void gui_manager::clear_canvas()
{
    circle_positions_.clear();
}

const std::vector<gui_manager::circle_positions>& gui_manager::get_circle_positions() const
{
    return circle_positions_;
}

void gui_manager::draw_toolbar(const std::size_t selected_color, const float brush_size) const
{
    constexpr int position_y = 15;

    DrawRectangle(0, 0, window_width_, toolbar_height, RAYWHITE);

    for (auto [x, y, size, color] : color_square_positions_) {

        DrawRectangle(x, y, size, size, color);

        // Draw a border around the currently selected color
        if (ColorIsEqual(color, colors_.at(selected_color)))
        {
            constexpr int thickness = 3;
            const float outline_size = static_cast<float>(size) + thickness * 2;
            const auto outline_position_x = (static_cast<float>(x) - thickness);
            constexpr int outline_position_y = position_y - thickness;

            const Rectangle outline_rect = { outline_position_x, outline_position_y, outline_size, outline_size };
            DrawRectangleLinesEx(outline_rect, thickness, BLACK);
        }
    }

    DrawText(std::format("Brush size: {}", brush_size).c_str(), color_square_bar_size_, position_y, 20, BLACK);
    DrawText("Press 'C' to clear the canvas.", color_square_bar_size_, position_y * 3, 20, BLACK);
}

std::optional<std::tuple<Color, int>> gui_manager::get_color_from_toolbar(const int x, const int y)
{
    int i = 0;
    for (const auto& [positionX, positionY, size, color] : color_square_positions_)
    {
        if (x >= positionX && x <= positionX + size && y >= positionY && y <= positionY + size)
        {
            return std::make_tuple(color, i);
        }
        i++;
    }
    return std::nullopt;
}

void gui_manager::set_window_width(const int width)
{
    window_width_ = width;
}
