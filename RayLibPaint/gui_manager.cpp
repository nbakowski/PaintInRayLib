//
// Created by Nikodem Bąkowski on 15/06/2026.
//

#include "gui_manager.h"

#include <format>
#include <string>

inline constexpr int square_size = 50;
inline constexpr int text_padding = 4;
inline constexpr int thickness = 3;
inline constexpr float outline_size = static_cast<float>(square_size) + thickness * 2;


namespace gui_helper_functions
{
    void draw_selected_outline(const int x, const int y)
    {
        const float outline_position_x = static_cast<float>(x) - thickness;
        const float outline_position_y = static_cast<float>(y) - thickness;

        const Rectangle outline_rect = {
            .x = outline_position_x,
            .y = outline_position_y,
            .width = outline_size,
            .height = outline_size
        };
        DrawRectangleLinesEx(outline_rect, thickness, BLACK);
    }
}

gui_manager::gui_manager(const int width, const std::vector<Color>& colors, const std::vector<int>& brush_sizes)
    : colors_(colors), brush_sizes_(brush_sizes), window_width_(width)
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
    for (const auto& [x, y, color] : color_square_positions_)
    {
        DrawRectangle(x, y, square_size, square_size, color);

        if (i == selected_color)
        {
            gui_helper_functions::draw_selected_outline(x, position_y);
        }
        i++;
    }
    
    std::size_t j = 0;
    for (const auto& [x, y, value] : size_square_positions_)
    {
        DrawRectangle(x, y, square_size, square_size, GRAY);
        DrawText(value.c_str(), x + text_padding, y + text_padding, 24, BLACK);
        
        if (j == selected_brush_size)
        {
            gui_helper_functions::draw_selected_outline(x, position_y);
        }
        j++;
    }
    DrawText("Press 'C' to clear the canvas.", color_square_bar_size_, position_y, 20, BLACK);
}

std::optional<gui_manager::color_pick_result> gui_manager::get_color_from_toolbar(const int x, const int y) const
{
    int i = 0;
    for (const auto& [square_x, square_y, square_color] : color_square_positions_)
    {
        if (x >= square_x && x <= square_x + square_size && y >= square_y && y <= square_y + square_size)
        {
            return color_pick_result{ 
            	.color = square_color,
            	.index = i 
            };
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
