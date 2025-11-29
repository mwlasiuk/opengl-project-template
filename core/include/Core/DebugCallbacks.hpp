#pragma once

#include <cstddef>
#include <cstdint>

namespace Core
{
    namespace DebugCallbacks
    {
        void DebugCallback_GLFW(const int32_t code, const char* message);
        void DebugCallback_OpenGL(const uint32_t source, const uint32_t type, const uint32_t id, const uint32_t severity, const int32_t length, const char* message, const void* user_data);
    }
}
