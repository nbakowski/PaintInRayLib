//
// Created by Nikodem Bąkowski on 15/06/2026.
//

#ifndef RAYLIB_GUI_MANAGER_H
#define RAYLIB_GUI_MANAGER_H
#include <optional>
#include <raylib.h>
#include <vector>
#include <array>

static int toolbar_height = 80;

class gui_manager
{
    struct circle_positions
    {
        int x;
        int y;
        Color color;
        float radius;
    };

    struct color_square_positions
    {
        int x;
        int y;
        int size;
        Color color;
    };

    int window_width_ = 0;
    int color_square_bar_size_;
    std::vector<color_square_positions> color_square_positions_;
    std::vector<circle_positions> circle_positions_;
    std::array<Color, 7> colors_ = { RED, GREEN, BLUE, YELLOW, MAGENTA, BLACK, WHITE };

public:
    explicit gui_manager(int width);
    [[nodiscard]] const std::array<Color, 7>& get_colors() const;
    void draw_circle_on_canvas(int x, int y, Color color, float radius);
    void clear_canvas();
    [[nodiscard]] const std::vector<circle_positions>& get_circle_positions() const;
    void draw_toolbar(std::size_t selected_color, float brush_size) const;
    std::optional<std::tuple<Color, int>> get_color_from_toolbar(int x, int y);
    void set_window_width(int width);
};

#endif //RAYLIB_GUI_MANAGER_H
