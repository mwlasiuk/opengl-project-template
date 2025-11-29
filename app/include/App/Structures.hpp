#pragma once

#include <Core/Structures.hpp>

#include <chrono>

namespace App
{
    struct CameraInputState
    {
        float last_mouse_position_x = 0.0;
        float last_mouse_position_y = 0.0;

        bool is_first_mouse = true;

        bool mouse_pressed_left  = false;
        bool mouse_pressed_right = false;
    };

    struct WindowUserData
    {
        struct
        {
            std::chrono::steady_clock::time_point last_time;

            float real_time       = 0.0f;
            float simulation_time = 0.0f;
            float delta_time      = 0.0f;
        } app_time;

        CameraInputState input  = {};
        Core::Camera     camera = {};

        struct
        {
            bool grid = true;

            float    origin_line_scale = 1.0f;
            uint32_t origin_line_width = 4;
            bool     origin            = true;

            float    target_line_scale = 0.1f;
            uint32_t target_line_width = 2;
            bool     target            = true;
        } draw;

        bool  camera_orthographic = false;
        float camera_fov_degrees  = 60.0f;

        float camera_orthographic_plane_near = -25.0f;
        float camera_orthographic_plane_far  = 100.0f;

        float camera_perspective_plane_near = 0.001f;
        float camera_perspective_plane_far  = 100.0f;

        int32_t window_iconified      = {};
        int32_t window_maximized      = {};
        int32_t window_focused        = {};
        int32_t window_cursor_entered = {};

        int32_t framebuffer_width  = {};
        int32_t framebuffer_height = {};

        bool is_paused = {};
    };
}
