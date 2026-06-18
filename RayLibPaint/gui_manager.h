//
// Created by Nikodem Bąkowski on 15/06/2026.
//
#pragma once
#include <optional>
#include <raylib.h>
#include <vector>
#include <array>

static int toolbar_height = 80;

class gui_manager
{
    struct color_square_positions
    {
        int x;
        int y;
        int size;
        Color color;
    };

    std::array<Color, 7> colors_;
    int window_width_ = 0;
    int color_square_bar_size_;
    std::vector<color_square_positions> color_square_positions_;

public:
    explicit gui_manager(int width, const std::array<Color, 7>& colors);
    void draw_toolbar(std::size_t selected_color, float brush_size) const;
    std::optional<std::tuple<Color, int>> get_color_from_toolbar(int x, int y);
    void set_window_width(int width);
};
