//
// Created by Nikodem Bąkowski on 15/06/2026.
//

#include "gui_manager.h"

#include <format>
#include <string>

inline constexpr int square_size = 50;
inline constexpr int text_padding = 4;

gui_manager::gui_manager(const int width, const std::vector<Color>& colors, const std::vector<int>& brush_sizes)
    : brush_sizes_(brush_sizes), colors_(colors), window_width_(width)
{
    int position_x = 15;
    for (const auto& color : colors_)
    {
        constexpr int position_y = 15;
        color_square_positions_.push_back({
            .x = position_x,
            .y = position_y,
            .color = color
        });
        position_x += 65;
    }
    
    for (const auto& size : brush_sizes_)
    {
        constexpr int square_size = 50;
        constexpr int position_y = 15;
        size_square_positions_.push_back({
            .x = position_x,
            .y = position_y,
            .value = std::to_string(size)
        });
        position_x += 65;
    }

    color_square_bar_size_ = position_x;
}

void gui_manager::draw_toolbar(const std::size_t selected_color, const std::size_t selected_brush_size) const
{
    constexpr int position_y = 15;

    DrawRectangle(0, 0, window_width_, toolbar_height, RAYWHITE);

    std::size_t i = 0;
    for (const auto& square : color_square_positions_)
    {
        DrawRectangle(square.x, square.y, square_size, square_size, square.color);

        if (i == selected_color)
        {
            constexpr int thickness = 3;
            const float outline_size = static_cast<float>(square_size) + thickness * 2;
            const float outline_position_x = static_cast<float>(square.x) - thickness;
            constexpr int outline_position_y = position_y - thickness;

            const Rectangle outline_rect = { outline_position_x, outline_position_y, outline_size, outline_size };
            DrawRectangleLinesEx(outline_rect, thickness, BLACK);
        }
        i++;
    }
    
    std::size_t j = 0;
    for (const auto& size_square : size_square_positions_)
    {
        DrawRectangle(size_square.x, size_square.y, square_size, square_size, GRAY);
        DrawText(size_square.value.c_str(), size_square.x + text_padding, size_square.y + text_padding, 24, BLACK);
        
        if (j == selected_brush_size)
        {
            constexpr int thickness = 3;
            const float outline_size = static_cast<float>(square_size) + thickness * 2;
            const float outline_position_x = static_cast<float>(size_square.x) - thickness;
            constexpr int outline_position_y = position_y - thickness;

            const Rectangle outline_rect = { outline_position_x, outline_position_y, outline_size, outline_size };
            DrawRectangleLinesEx(outline_rect, thickness, BLACK);
        }
        j++;
    }
    DrawText("Press 'C' to clear the canvas.", color_square_bar_size_, position_y, 20, BLACK);
}

std::optional<gui_manager::color_pick_result> gui_manager::get_color_from_toolbar(const int x, const int y) const
{
    int i = 0;
    for (const auto& square : color_square_positions_)
    {
        if (x >= square.x && x <= square.x + square_size && y >= square.y && y <= square.y + square_size)
        {
            return color_pick_result{ square.color, i };
        }
        i++;
    }
    return std::nullopt;
}

std::optional<int> gui_manager::get_brush_size_from_toolbar(const int x, const int y) const
{
    int i = 0;
    for (const auto& square : size_square_positions_)
    {
        if (x >= square.x && x <= square.x + square_size && y >= square.y && y <= square.y + square_size)
        {
            return i;
        }
        i++;
    }
    return std::nullopt;
}

void gui_manager::set_window_width(const int width)
{
    window_width_ = width;
}
