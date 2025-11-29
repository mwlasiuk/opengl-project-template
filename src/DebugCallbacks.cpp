#include "DebugCallbacks.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <spdlog/spdlog.h>

namespace DebugCallbacks
{
    void glfw_error_callback(const int32_t code, const char* message)
    {
        spdlog::critical("GLFW error {:#X} : {}", code, message);
    }

    void opengl_error_callback(const uint32_t source, const uint32_t type, const uint32_t id, const uint32_t severity, const int32_t length, const char* message, const void* user_data)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            {
                spdlog::error("OpenGL message (id={:#X} source={:#X} type={} severity={:#X}): {}", id, source, type, severity, message);
                break;
            }
        case GL_DEBUG_SEVERITY_MEDIUM:
            {
                spdlog::warn("OpenGL message (id={:#X} source={:#X} type={} severity={:#X}): {}", id, source, type, severity, message);
                break;
            }
        case GL_DEBUG_SEVERITY_LOW:
            {
                spdlog::info("OpenGL message (id={:#X} source={:#X} type={} severity={:#X}): {}", id, source, type, severity, message);
                break;
            }
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            {
                spdlog::debug("OpenGL message (id={:#X} source={:#X} type={} severity={:#X}): {}", id, source, type, severity, message);
                break;
            }
        default:
            {
                spdlog::critical("OpenGL message (id={:#X} source={:#X} type={} severity={:#X}): {}", id, source, type, severity, message);
                break;
            }
        }
    }
}