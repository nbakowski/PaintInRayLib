//
// Created by Nikodem Bąkowski on 18/06/2026.
//
#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <array>
#include <vector>
#include <raylib.h>

class settings_manager
{
    struct circle_positions
    {
        int x;
        int y;
        Color color;
        float radius;
    };
	std::vector<circle_positions> circle_positions_;
	std::array<Color, 7> colors_ = { RED, GREEN, BLUE, YELLOW, MAGENTA, BLACK, WHITE };

public:
    void clear_canvas();
    void draw_circle_on_canvas(int x, int y, Color color, float radius);
    [[nodiscard]] const std::vector<circle_positions>& get_circle_positions() const;
    [[nodiscard]] const std::array<Color, 7>& get_colors() const;
};
#endif