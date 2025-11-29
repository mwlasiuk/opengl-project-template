#pragma once

#include <cstdint>

namespace DebugCallbacks
{
    void glfw_error_callback(const int32_t code, const char* message);
    void opengl_error_callback(const uint32_t source, const uint32_t type, const uint32_t id, const uint32_t severity, const int32_t length, const char* message, const void* user_data);
}
