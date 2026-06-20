//
// Created by Nikodem Bąkowski on 15/06/2026.
//
#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <optional>
#include <raylib.h>
#include <vector>

inline constexpr int toolbar_height = 80;

class gui_manager
{
    struct color_square_positions
    {
        int x;
        int y;
        int size;
        Color color;
    };

    std::vector<Color> colors_;
    int window_width_ = 0;
    int color_square_bar_size_;
    std::vector<color_square_positions> color_square_positions_;

public:
    struct color_pick_result
    {
        Color color;
        int index;
    };

    explicit gui_manager(int width, const std::vector<Color>& colors);
    void draw_toolbar(std::size_t selected_color, float brush_size) const;
    std::optional<color_pick_result> get_color_from_toolbar(int x, int y) const;
    void set_window_width(int width);
};
#endif
