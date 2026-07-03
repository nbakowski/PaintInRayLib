#include "raylib.h"
#include "gui_manager.h"

#include <vector>
#include <array>
#include <filesystem>
#include <format>
#include <optional>

#include "paint_canvas.h"

using color_pick_result = gui_manager::color_pick_result;

inline constexpr auto min_window_width = 800;
inline constexpr auto min_window_height = 600;
inline constexpr auto screenshot_delay = 2;
inline constexpr std::string_view file_extension = "png";

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

    // Get the amount of files that have been previously saved
    // Avoid overwriting already existing files
    int get_files_count()
    {
        auto file_counter = 0;
        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
        {
            if (entry.path().filename() == std::format("file{}.{}", file_counter, file_extension))
            {
                file_counter++;
            }
            else
            {
                break;
            }
        }
        return file_counter;
    }

    bool is_value_in_range(const int value, const int lower_bound, const int upper_bound)
    {
        return value >= lower_bound && value <= upper_bound;
    }
}

int main()
{
    constexpr int screen_width = 1270;
    constexpr int screen_height = 720;

    // Set window options
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "SigmaPaint");
    SetWindowMinSize(min_window_width, min_window_height);

    uint8_t screenshot_delay_counter = 0;
    int screenshot_amount_counter = get_files_count();
    bool is_screenshot_taken = false;

    paint_canvas canvas;
    const auto& colors = canvas.get_colors();
    const auto& brush_sizes = canvas.get_brush_sizes();
    auto gui = gui_manager(
        screen_width,
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

        // Screenshot as a file saving mechanism 
        if (IsKeyPressed(KEY_S) || is_screenshot_taken)
        {
            if (screenshot_delay_counter == screenshot_delay)
            {

                std::string file_name = std::format("file{}.{}", screenshot_amount_counter, file_extension);
                TakeScreenshot(file_name.c_str());
                screenshot_delay_counter = 0;
                screenshot_amount_counter++;
                is_screenshot_taken = false;
            }
            else
            {
                is_screenshot_taken = true;
                screenshot_delay_counter++;
            }
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

            const auto save_icon_positions = gui.get_save_icon_position();
            const int icon_x = get<0>(save_icon_positions);
            const int icon_y = get<1>(save_icon_positions);
            
            if (
                const int square_size = get<2>(save_icon_positions);
                is_value_in_range(mouse_x, icon_x, icon_x + square_size) && is_value_in_range(mouse_y, icon_y, icon_y + square_size)
                )
            {
                is_screenshot_taken = true;
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
        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) 
        {
            color_index = (color_index + 1) % colors.size();
            current_color = colors[color_index];
        }

        if (IsKeyPressed(KEY_C)) { canvas.clear_canvas(); }

        // Draw the buffer for previously drawn circles
        for (const auto& [x, y, color, radius] : canvas.get_circle_positions())
        {
            DrawCircle(x, y, radius, color);
        }

        // Draw the GUI
        // Turn the GUI off for the screenshot
        if (!is_screenshot_taken)
        {
            draw_brush_at_mouse_position(mouse_x, mouse_y, static_cast<float>(current_brush_size), current_color);
            gui.draw_gui(color_index, brush_size_index, mouse_x, mouse_y);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
