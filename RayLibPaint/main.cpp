#include "raylib.h"
#include "gui_manager.h"

#include <vector>
#include <array>
#include <format>
#include <optional>

#include "settings_manager.h"

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

    std::optional<std::tuple<Color, int>> check_for_toolbar_actions(const int x, const int y, gui_manager& g)
    {
        if (y <= toolbar_height)
        {
            ShowCursor();
            if (
                auto return_color = g.get_color_from_toolbar(x, y);
                return_color.has_value() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
                )
            {
                return return_color;
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

    auto settings = settings_manager();
    const auto& colors = settings.get_colors();
    auto gui = gui_manager(
        1270,
        std::vector(
            std::begin(colors),
            std::end(colors)
        )
    );

    Color current_color = settings.get_colors().front();
    std::size_t color_index = 0;

    // ReSharper disable once CppTooWideScope
    constexpr std::array<float, 6> brush_sizes = { 5, 15, 20, 25, 50, 100 };
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
        const float current_brush_size = brush_sizes.at(brush_size_index);

        if (current_width != GetScreenWidth())
        {
            gui.set_window_width(GetScreenWidth());
            current_width = GetScreenWidth();
        }

        BeginDrawing();

        ClearBackground(WHITE);

        if (auto color_and_index = check_for_toolbar_actions(mouse_x, mouse_y, gui))
        {
            current_color = std::get<0>(color_and_index.value());
            color_index = std::get<1>(color_and_index.value());
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsCursorHidden())
        {
            settings.draw_circle_on_canvas(mouse_x, mouse_y, current_color, current_brush_size);
        }

        // Change the brush size
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            constexpr float time_between_clicks = 0.2f;
            if (const double click_time = GetTime(); click_time - last_right_click > time_between_clicks)
            {
                brush_size_index = (brush_size_index + 1) % brush_sizes.size();
                last_right_click = GetTime();
            }
        }

        // Cycle through colors with middle mouse button, really cool!
        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
            color_index = (color_index + 1) % settings.get_colors().size();
            current_color = settings.get_colors().at(color_index);
        }

        if (IsKeyPressed(KEY_C)) { settings.clear_canvas(); }

        // Draw the buffer for previously drawn circles
        for (const auto& [x, y, color, radius] : settings.get_circle_positions())
        {
            DrawCircle(x, y, radius, color);
        }

        // Draw the brush
        draw_brush_at_mouse_position(mouse_x, mouse_y, current_brush_size, current_color);
        gui.draw_toolbar(color_index, brush_sizes.at(brush_size_index));

        EndDrawing();
    }

    CloseWindow();

    return 0;
}