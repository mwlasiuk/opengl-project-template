#pragma once

#include <utility>

#include <glm/glm.hpp>

namespace Core
{
    struct AABB
    {
        glm::f32vec3 corner_min;
        glm::f32vec3 corner_max;
    };

    struct Ray
    {
        glm::f32vec3 origin;
        glm::f32vec3 direction;
    };

    struct Point2D
    {
        glm::f32vec2 position;
    };

    struct Point3D
    {
        glm::f32vec3 position;
    };

    struct ColorPoint3D
    {
        glm::f32vec3 position;
        glm::u8vec4  color;
    };

    struct SystemTransformUniforms
    {
        glm::f32mat4x4 preview_view_projection;
        glm::f32mat4x4 preview_projection;
        glm::f32mat4x4 preview_view;
    };

    enum class UpAxis : int32_t
    {
        X       = 0x1,
        Y       = 0x2,
        Z       = 0x4,
        DEFAULT = Y
    };
}

namespace Core
{
    struct VertexBufferAttributeLayout
    {
        uint32_t location;
        int32_t  components;
        uint32_t type;
        int32_t  normalize;
        int32_t  stride;
        int32_t  offset;
    };

    struct TextureSpecification
    {
        uint32_t width  = UINT32_MAX;
        uint32_t height = UINT32_MAX;

        uint32_t opengl_internal_format = UINT32_MAX;
        uint32_t opengl_format          = UINT32_MAX;

        uint32_t opengl_type = UINT32_MAX;

        struct
        {
            uint32_t opengl_texture_min_filter = 0x2702;
            uint32_t opengl_texture_mag_filter = 0x2601;
            uint32_t image_wrap_s              = 0x2901;
            uint32_t image_wrap_t              = 0x2901;
            uint32_t image_wrap_r              = 0x2901;
        } sampler_parameters;
    };

    struct ShaderDescriptor
    {
        uint32_t    shader_type = 0;
        int32_t     source_size = 0;
        const char* source      = nullptr;
    };

    struct RAMInfo
    {
        uint32_t total_memory_mb = 0;
        uint32_t used_memory_mb  = 0;
        uint32_t free_memory_mb  = 0;
    };

    struct ProcessRAMInfo
    {
        uint32_t memory_used_mb    = 0;
        uint32_t virtual_memory_mb = 0;
    };

    struct GPUInfo
    {
        const char* gl_version                  = nullptr;
        const char* gl_vendor                   = nullptr;
        const char* gl_renderer                 = nullptr;
        const char* gl_shading_language_version = nullptr;
    };

    struct GPUMemoryInfo
    {
        bool has_memory_info_nvx = false;
        bool has_memory_info_ati = false;

        struct NVXPool
        {
            uint32_t dedicated_mb   = 0;
            uint32_t total_mb       = 0;
            uint32_t available_mb   = 0;
            uint32_t eviction_count = 0;
            uint32_t evicted_mb     = 0;
        };

        // ATI pools (values[4] returned by extension)
        struct ATIPool
        {
            uint32_t free_mb                   = 0;
            uint32_t largest_free_mb           = 0;
            uint32_t total_auxiliary_free_mb   = 0;
            uint32_t largest_auxiliary_free_mb = 0;
        };

        NVXPool nvx_pool;

        ATIPool texture;
        ATIPool vbo;
        ATIPool renderbuffer;
    };
}

namespace Core
{
    typedef std::vector<VertexBufferAttributeLayout> VertexBufferLayout;
    typedef std::vector<ShaderDescriptor>            ProgramDescriptor;
}

namespace Core
{
    [[nodiscard]] static inline glm::f32vec3 UpAxisToGLM(const UpAxis axis)
    {
        switch (axis)
        {
        case UpAxis::X:
            {
                return glm::f32vec3(1.0f, 0.0f, 0.0f);
            }
        case UpAxis::Y:
            {
                return glm::f32vec3(0.0f, 1.0f, 0.0f);
            }
        case UpAxis::Z:
            {
                return glm::f32vec3(0.0f, 0.0f, 1.0f);
            }
        default:
            {
                std::unreachable();
            }
        }
    }
}
