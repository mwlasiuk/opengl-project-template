#pragma once

#include <Core/Structures.hpp>

#include <Core/OpenGL/Buffer.hpp>
#include <Core/OpenGL/Program.hpp>
#include <Core/OpenGL/VertexArray.hpp>

#include <Core/Camera.hpp>

#include <span>

namespace Core
{
    struct DynamicRendererRenderPassInfo
    {
        bool     is_orthographic                = {};
        Camera*  camera                         = {};
        uint32_t width                          = {};
        uint32_t height                         = {};
        float    fov_degrees                    = {};
        float    camera_orthographic_plane_near = {};
        float    camera_orthographic_plane_far  = {};
        float    camera_perspective_plane_near  = {};
        float    camera_perspective_plane_far   = {};
    };

    class DynamicRenderer
    {
    private:
        const size_t _max_points       = {};
        const size_t _max_color_points = {};

    private:
        size_t _pass_upload_offset_point3d       = 0;
        size_t _pass_upload_offset_color_point3d = 0;

    private:
        Buffer* _buffer_system_transforms = nullptr;

        uint32_t       _last_mode_point3d    = 0;
        glm::f32mat4x4 _last_model_point3d   = glm::identity<glm::f32mat4x4>();
        glm::f32vec4   _last_color_point3d   = glm::one<glm::f32vec4>();
        Program*       _program_point3d      = nullptr;
        Buffer*        _buffer_point3d       = nullptr;
        VertexArray*   _vertex_array_point3d = nullptr;

        uint32_t       _last_mode_color_point3d    = 0;
        glm::f32mat4x4 _last_model_color_point3d   = glm::identity<glm::f32mat4x4>();
        Program*       _program_color_point3d      = nullptr;
        Buffer*        _buffer_color_point3d       = nullptr;
        VertexArray*   _vertex_array_color_point3d = nullptr;

    public:
        explicit DynamicRenderer(const size_t max_points, const size_t max_color_points);
        ~DynamicRenderer();

        void RenderPassBegin(const DynamicRendererRenderPassInfo& dynamic_renderer_render_pass_info);
        void RenderPassEnd();

        void SubmitPoint3D(const uint32_t mode, const Point3D* const points, const size_t count, const glm::f32vec4& color = glm::one<glm::f32vec4>(), const glm::f32mat4x4& model = glm::identity<glm::f32mat4x4>());
        void SubmitPoint3D(const uint32_t mode, std::span<const Point3D> points, const glm::f32vec4& color = glm::one<glm::f32vec4>(), const glm::f32mat4x4& model = glm::identity<glm::f32mat4x4>());

        void SubmitColorPoint3D(const uint32_t mode, const ColorPoint3D* const points, const size_t count, const glm::f32mat4x4& model = glm::identity<glm::f32mat4x4>());
        void SubmitColorPoint3D(const uint32_t mode, std::span<const ColorPoint3D> points, const glm::f32mat4x4& model = glm::identity<glm::f32mat4x4>());

    private:
        void flush_point3d();
        void flush_color_point3d();
    };
}
