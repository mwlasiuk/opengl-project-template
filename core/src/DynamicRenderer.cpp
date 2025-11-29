#include <Core/Renderer/DynamicRenderer.hpp>

#include <Core/Default/DefaultShaders.hpp>
#include <Core/Default/DefaultVertexLayouts.hpp>

#include <glad/glad.h>

namespace Core
{
    static inline size_t primitive_multiple(const uint32_t mode)
    {
        switch (mode)
        {
        case GL_POINTS:
            {
                return 1;
            }
        case GL_LINES:
            {
                return 2;
            }
        case GL_TRIANGLES:
            {
                return 3;
            }
        default:
            {
                std::unreachable();
            }
        }
    }

    static inline bool valid_count(const uint32_t mode, const size_t count)
    {
        const size_t multiple = primitive_multiple(mode);

        return multiple && (count % multiple == 0U);
    }
}

namespace Core
{
    DynamicRenderer::DynamicRenderer(const size_t max_points, const size_t max_color_points)
        : _max_points(max_points),
          _max_color_points(max_color_points)
    {
        const ProgramDescriptor program_point_descriptor = {
            ShaderDescriptor{.shader_type = GL_VERTEX_SHADER,
                             .source_size = static_cast<int32_t>(Default::PointVertexSize()),
                             .source      = Default::PointVertexCode()},
            ShaderDescriptor{.shader_type = GL_FRAGMENT_SHADER,
                             .source_size = static_cast<int32_t>(Default::PointFragmentSize()),
                             .source      = Default::PointFragmentCode()}};

        const ProgramDescriptor program_color_point_descriptor = {
            ShaderDescriptor{.shader_type = GL_VERTEX_SHADER,
                             .source_size = static_cast<int32_t>(Default::ColorPointVertexSize()),
                             .source      = Default::ColorPointVertexCode()},
            ShaderDescriptor{.shader_type = GL_FRAGMENT_SHADER,
                             .source_size = static_cast<int32_t>(Default::ColorPointFragmentSize()),
                             .source      = Default::ColorPointFragmentCode()}};

        _buffer_system_transforms = new Buffer(GL_DYNAMIC_STORAGE_BIT, sizeof(SystemTransformUniforms), nullptr);

        _program_point3d      = new Program(program_point_descriptor);
        _buffer_point3d       = new Buffer(GL_DYNAMIC_STORAGE_BIT, _max_points * sizeof(Point3D), nullptr);
        _vertex_array_point3d = new VertexArray(_buffer_point3d, false, nullptr, false, Default::GetVertexLayout<Point3D>());

        _program_color_point3d      = new Program(program_color_point_descriptor);
        _buffer_color_point3d       = new Buffer(GL_DYNAMIC_STORAGE_BIT, _max_color_points * sizeof(ColorPoint3D), nullptr);
        _vertex_array_color_point3d = new VertexArray(_buffer_color_point3d, false, nullptr, false, Default::GetVertexLayout<ColorPoint3D>());

        _buffer_system_transforms->SetAsShaderResource(GL_UNIFORM_BUFFER, 0, sizeof(SystemTransformUniforms), 0);
    }

    DynamicRenderer::~DynamicRenderer()
    {
        delete _vertex_array_color_point3d;
        delete _buffer_color_point3d;
        delete _program_color_point3d;

        delete _vertex_array_point3d;
        delete _buffer_point3d;
        delete _program_point3d;

        delete _buffer_system_transforms;
    }

    void DynamicRenderer::RenderPassBegin(const DynamicRendererRenderPassInfo& dynamic_renderer_render_pass_info)
    {
        const bool     is_orthographic                = dynamic_renderer_render_pass_info.is_orthographic;
        const Camera*  camera                         = dynamic_renderer_render_pass_info.camera;
        const uint32_t width                          = dynamic_renderer_render_pass_info.width;
        const uint32_t height                         = dynamic_renderer_render_pass_info.height;
        const float    fov_degrees                    = dynamic_renderer_render_pass_info.fov_degrees;
        const float    camera_orthographic_plane_near = dynamic_renderer_render_pass_info.camera_orthographic_plane_near;
        const float    camera_orthographic_plane_far  = dynamic_renderer_render_pass_info.camera_orthographic_plane_far;
        const float    camera_perspective_plane_near  = dynamic_renderer_render_pass_info.camera_perspective_plane_near;
        const float    camera_perspective_plane_far   = dynamic_renderer_render_pass_info.camera_perspective_plane_far;

        const glm::f32mat4x4 projection = [&]()
        {
            const float aspect = static_cast<float>(width) / static_cast<float>(height);
            const float fov    = glm::radians(fov_degrees);

            if (is_orthographic)
            {
                const float distance            = camera->GetDistance();
                const float orthographic_height = 2.0f * distance * glm::tan(fov * 0.5f);
                const float orthographic_width  = orthographic_height * aspect;

                return glm::ortho(
                    -orthographic_width * 0.5f,
                    orthographic_width * 0.5f,
                    -orthographic_height * 0.5f,
                    orthographic_height * 0.5f,
                    camera_orthographic_plane_near,
                    camera_orthographic_plane_far);
            }

            return glm::perspective(
                fov,
                aspect,
                camera_perspective_plane_near,
                camera_perspective_plane_far);
        }();

        const glm::f32mat4x4 view = camera->GetViewMatrix();

        SystemTransformUniforms system_transform_uniforms = {};
        system_transform_uniforms.preview_view_projection = projection * view;
        system_transform_uniforms.preview_projection      = projection;
        system_transform_uniforms.preview_view            = view;

        _buffer_system_transforms->Upload(&system_transform_uniforms, sizeof(SystemTransformUniforms));
    }

    void DynamicRenderer::RenderPassEnd()
    {
        flush_point3d();
        flush_color_point3d();
    }

    void DynamicRenderer::SubmitPoint3D(const uint32_t mode, const Point3D* const points, const size_t count, const glm::f32vec4& color, const glm::f32mat4x4& model)
    {
        assert(valid_count(mode, count) && "Invalid primitive vertex count");

        if (model != _last_model_point3d || color != _last_color_point3d || mode != _last_mode_point3d)
        {
            flush_point3d();
            _last_model_point3d = model;
            _last_color_point3d = color;
            _last_mode_point3d  = mode;
        }

        size_t offset    = 0;
        size_t remaining = count;

        while (remaining > 0)
        {
            const size_t space   = _max_points - _pass_upload_offset_point3d;
            const size_t to_copy = std::min(remaining, space);

            if (to_copy == 0)
            {
                flush_point3d();
                _last_model_point3d = model;
                continue;
            }

            const Point3D* upload_source_offset      = points + offset;
            const size_t   upload_size               = to_copy * sizeof(Point3D);
            const size_t   upload_destination_offset = _pass_upload_offset_point3d * sizeof(Point3D);

            _buffer_point3d->Upload(upload_source_offset, upload_size, upload_destination_offset);

            _pass_upload_offset_point3d += to_copy;
            offset += to_copy;
            remaining -= to_copy;
        }
    }

    void DynamicRenderer::SubmitPoint3D(const uint32_t mode, std::span<const Point3D> points, const glm::f32vec4& color, const glm::f32mat4x4& model)
    {
        SubmitPoint3D(mode, points.data(), points.size(), color, model);
    }

    void DynamicRenderer::SubmitColorPoint3D(const uint32_t mode, const ColorPoint3D* const points, const size_t count, const glm::f32mat4x4& model)
    {
        assert(valid_count(mode, count) && "Invalid primitive vertex count");

        if (model != _last_model_color_point3d || mode != _last_mode_color_point3d)
        {
            flush_color_point3d();
            _last_model_color_point3d = model;
            _last_mode_color_point3d  = mode;
        }

        size_t offset    = 0;
        size_t remaining = count;

        while (remaining > 0)
        {
            const size_t space   = _max_color_points - _pass_upload_offset_color_point3d;
            const size_t to_copy = std::min(remaining, space);

            if (to_copy == 0)
            {
                flush_color_point3d();
                _last_model_color_point3d = model;
                continue;
            }

            const ColorPoint3D* upload_source_offset      = points + offset;
            const size_t        upload_size               = to_copy * sizeof(ColorPoint3D);
            const size_t        upload_destination_offset = _pass_upload_offset_color_point3d * sizeof(ColorPoint3D);

            _buffer_color_point3d->Upload(upload_source_offset, upload_size, upload_destination_offset);

            _pass_upload_offset_color_point3d += to_copy;
            offset += to_copy;
            remaining -= to_copy;
        }
    }

    void DynamicRenderer::SubmitColorPoint3D(const uint32_t mode, std::span<const ColorPoint3D> points, const glm::f32mat4x4& model)
    {
        SubmitColorPoint3D(mode, points.data(), points.size(), model);
    }

    void DynamicRenderer::flush_point3d()
    {
        if (!_pass_upload_offset_point3d)
        {
            return;
        }

        _program_point3d->PushUniform16F32("uniform_Model", _last_model_point3d);

        _program_point3d->Bind();
        _vertex_array_point3d->Bind();
        _vertex_array_point3d->DrawArray(_last_mode_point3d, static_cast<uint32_t>(_pass_upload_offset_point3d));
        _vertex_array_point3d->Unbind();
        _program_point3d->Unbind();

        _pass_upload_offset_point3d = 0;
        _last_mode_point3d          = GL_POINTS;
        _last_color_point3d         = glm::one<glm::f32vec4>();
        _last_model_point3d         = glm::identity<glm::f32mat4x4>();
    }

    void DynamicRenderer::flush_color_point3d()
    {
        if (!_pass_upload_offset_color_point3d)
        {
            return;
        }

        _program_color_point3d->PushUniform16F32("uniform_Model", _last_model_color_point3d);

        _program_color_point3d->Bind();
        _vertex_array_color_point3d->Bind();
        _vertex_array_color_point3d->DrawArray(_last_mode_color_point3d, static_cast<uint32_t>(_pass_upload_offset_color_point3d));
        _vertex_array_color_point3d->Unbind();
        _program_color_point3d->Unbind();

        _pass_upload_offset_color_point3d = 0;
        _last_mode_color_point3d          = GL_POINTS;
        _last_model_color_point3d         = glm::identity<glm::f32mat4x4>();
    }
}
