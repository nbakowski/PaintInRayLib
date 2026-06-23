//
// Created by Nikodem Bąkowski on 15/06/2026.
//
#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <optional>
#include <raylib.h>
#include <vector>
#include <string>

inline constexpr int toolbar_height = 80;

class gui_manager
{
    struct color_square_positions
    {
        int x;
        int y;
        Color color;
    };
    
    struct size_square_positions
    {
        int x;
        int y;
        std::string value;
    };

    std::vector<Color> colors_;
    std::vector<int> brush_sizes_;
    int window_width_ = 0;
    int color_square_bar_size_;
    std::vector<color_square_positions> color_square_positions_;
    std::vector<size_square_positions> size_square_positions_;

public:
    struct color_pick_result
    {
        Color color;
        int index;
    };

    explicit gui_manager(int width, const std::vector<Color>& colors, const std::vector<int>& brush_sizes);
    void draw_gui(std::size_t selected_color, std::size_t selected_brush_size, int mouse_x, int mouse_y) const;
    [[nodiscard]] std::optional<color_pick_result> get_color_from_toolbar(int x, int y) const;
    [[nodiscard]] std::optional<int> get_brush_size_from_toolbar(int x, int y) const;
    void set_window_width(int width);
};
#endif
