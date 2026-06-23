#include "raylib.h"
#include "gui_manager.h"

#include <vector>
#include <array>
#include <format>
#include <optional>

#include "paint_canvas.h"

using color_pick_result = gui_manager::color_pick_result;

constexpr auto min_window_width = 800;
constexpr auto min_window_height = 600;

namespace
{
    void draw_brush_at_mouse_position(const int x, const int y, const float radius, const Color color)
    {
        if (ColorIsEqual(color, WHITE))
        {
            DrawCircleLines(x, y, radius, BLACK);
        }
        DrawCircle(x, y, radius, color);
    }

    std::optional<color_pick_result> check_for_toolbar_actions(const int x, const int y, const gui_manager& g)
    {
        if (y <= toolbar_height)
        {
            ShowCursor();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (const auto return_color = g.get_color_from_toolbar(x, y);
                    return_color.has_value())
                {
                    return return_color;
                }
            }
        }
        else
        {
            HideCursor();
        }
        return std::nullopt;
    }
}

int main()
{
    constexpr int screen_width = 1270;
    constexpr int screen_height = 720;

    paint_canvas canvas;
    const auto& colors = canvas.get_colors();
    const auto& brush_sizes = canvas.get_brush_sizes();
    auto gui = gui_manager(
        1270,
        std::vector(
            std::begin(colors),
            std::end(colors)
        ),
        std::vector(
            std::begin(brush_sizes),
            std::end(brush_sizes)
        )
    );

    Color current_color = colors.front();
    std::size_t color_index = 0;

    std::size_t brush_size_index = 0;
    double last_right_click = 0;

    // Set window options
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "SigmaPaint");
    SetWindowMinSize(min_window_width, min_window_height);

    int current_width = GetScreenWidth();

    HideCursor();

    while (!WindowShouldClose())
    {
        const int mouse_x = GetMouseX();
        const int mouse_y = GetMouseY();
        const int current_brush_size = canvas.get_brush_sizes()[brush_size_index];

        if (current_width != GetScreenWidth())
        {
            gui.set_window_width(GetScreenWidth());
            current_width = GetScreenWidth();
        }

        BeginDrawing();

        ClearBackground(WHITE);

        if (const auto color_and_index = check_for_toolbar_actions(mouse_x, mouse_y, gui))
        {
            current_color = color_and_index->color;
            color_index = color_and_index->index;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (auto brush_size_index_result = gui.get_brush_size_from_toolbar(mouse_x, mouse_y))
            {
                brush_size_index = brush_size_index_result.value();
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsCursorHidden())
        {
            canvas.draw_circle_on_canvas(mouse_x, mouse_y, current_color, static_cast<float>(current_brush_size));
        }

        // Change the brush size
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            constexpr float time_between_clicks = 0.2f;
            if (const double click_time = GetTime(); click_time - last_right_click > time_between_clicks)
            {
                brush_size_index = (brush_size_index + 1) % canvas.get_brush_sizes().size();
                last_right_click = GetTime();
            }
        }

        // Cycle through colors with middle mouse button, really cool!
        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
            color_index = (color_index + 1) % colors.size();
            current_color = colors[color_index];
        }

        if (IsKeyPressed(KEY_C)) { canvas.clear_canvas(); }

        // Draw the buffer for previously drawn circles
        for (const auto& [x, y, color, radius] : canvas.get_circle_positions())
        {
            DrawCircle(x, y, radius, color);
        }

        // Draw the brush
        draw_brush_at_mouse_position(mouse_x, mouse_y, static_cast<float>(current_brush_size), current_color);
        gui.draw_gui(color_index, brush_size_index, mouse_x, mouse_y);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
