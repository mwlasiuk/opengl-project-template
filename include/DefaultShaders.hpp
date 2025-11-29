#pragma once

#include <string>

///
/// --- --- --- --- ---
///

static const std::string kDefaultShader_ColorPoint_Vertex = R"(
    #version 460 core

    struct SystemTransformUniforms
    {
        mat4 preview_projection;
        mat4 preview_view;
    };

    layout(binding = 0, std140) uniform SystemTransformUniformsUniformBuffer
    {
        SystemTransformUniforms system_transform_uniforms;
    };

    layout(location = 0) in vec3 attribute_Position;
    layout(location = 1) in vec4 attribute_Color;

    layout(location = 0) uniform mat4 uniform_Pose = mat4(1.0f);

    layout(location = 0) out DATA
    {
        vec4 color;
    }
    vs_out;

    void main()
    {
        vs_out.color = attribute_Color;

        gl_Position = system_transform_uniforms.preview_projection * system_transform_uniforms.preview_view * uniform_Pose * vec4(attribute_Position, 1.0f);
    }
)";

static const std::string kDefaultShader_ColorPoint_Fragment = R"(
    #version 460 core

    layout(location = 0) out vec4 attachment_ColorOutput;

    layout(location = 0) in DATA
    {
        vec4 color;
    }
    fs_in;

    void main()
    {
        attachment_ColorOutput = fs_in.color;
    }
)";

///
/// --- --- --- --- ---
///

static const std::string kDefaultShader_Point_Vertex = R"(
    #version 460 core

    struct SystemTransformUniforms
    {
        mat4 preview_projection;
        mat4 preview_view;
    };

    layout(binding = 0, std140) uniform SystemTransformUniformsUniformBuffer
    {
        SystemTransformUniforms system_transform_uniforms;
    };

    layout(location = 0) in vec3 attribute_Position;
    
    layout(location = 0) uniform mat4 uniform_Pose = mat4(1.0f);

    void main()
    {
        gl_Position = system_transform_uniforms.preview_projection * system_transform_uniforms.preview_view * uniform_Pose * vec4(attribute_Position, 1.0f);
    }
)";

static const std::string kDefaultShader_Point_Fragment = R"(
    #version 460 core

    layout(location = 0) out vec4 attachment_ColorOutput;

    layout(location = 1) uniform vec4 uniform_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    void main()
    {
        attachment_ColorOutput = uniform_Color;
    }
)";

///
/// --- --- --- --- ---
///

static const std::string kDefaultShader_Grid_Vertex = R"(
    #version 450 core

    struct SystemTransformUniforms
    {
        mat4 preview_projection;
        mat4 preview_view;
    };

    layout(binding = 0, std140) uniform SystemTransformUniformsUniformBuffer
    {
        SystemTransformUniforms system_transform_uniforms;
    };

    layout(location = 0) in vec2 attribute_position;

    out DATA
    {
        vec3 point_near;
        vec3 point_far;
    }
    vs_out;

    vec3 unproject_point(in const vec3 ndc)
    {
        mat4 inv_proj = inverse(system_transform_uniforms.preview_projection);
        mat4 inv_view = inverse(system_transform_uniforms.preview_view);

        vec4 clip = vec4(ndc, 1.0);
        vec4 view = inv_proj * clip;
        view /= view.w;

        vec4 world = inv_view * view;

        return world.xyz;
    }

    void main()
    {
        vec3 ndc_near = vec3(attribute_position, -1.0);
        vec3 ndc_far  = vec3(attribute_position, 1.0);

        vs_out.point_near = unproject_point(ndc_near);
        vs_out.point_far  = unproject_point(ndc_far);

        gl_Position = vec4(attribute_position, 0.0, 1.0);
    }
)";

static const std::string kDefaultShader_Grid_Fragment = R"(
    #version 450 core

    layout(location = 0) out vec4 attachment_ColorOutput;

    layout(location = 1) uniform float uniform_GridSpacing = 1.0f;
    layout(location = 2) uniform vec4 uniform_ColorMinor   = vec4(0.35f, 0.35f, 0.35f, 1.0f);
    layout(location = 3) uniform vec4 uniform_ColorMajor   = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    layout(location = 4) uniform float uniform_FadeStart = 20.0f;
    layout(location = 5) uniform float uniform_FadeEnd   = 80.0f;

    layout(location = 6) uniform int uniform_CameraUpAxis = 2;

    in DATA
    {
        vec3 point_near;
        vec3 point_far;
    } fs_in;

    float grid(in const vec2 coord, in const float spacing)
    {
        const vec2 p = coord / spacing;
        const vec2 g = abs(fract(p - 0.5) - 0.5) / max(fwidth(p), 0.0001);

        const float line = min(g.x, g.y);

        return 1.0 - min(line, 1.0);
    }

    void main()
    {
        float t = 0.0f;

        if (uniform_CameraUpAxis == 0x1) // X-up
        {
            t = -fs_in.point_near.x / (fs_in.point_far.x - fs_in.point_near.x);
        }
        else if (uniform_CameraUpAxis == 0x2) // Y-up
        {
            t = -fs_in.point_near.y / (fs_in.point_far.y - fs_in.point_near.y);
        }
        else if (uniform_CameraUpAxis == 0x4) // Z-up
        {
            t = -fs_in.point_near.z / (fs_in.point_far.z - fs_in.point_near.z);
        }
        else
        {
            discard;
        }

        if (t < 0.0)
        {
            discard;
        }

        const vec3 world_position = fs_in.point_near + t * (fs_in.point_far - fs_in.point_near);

        vec2 coord = vec2(0.0f, 0.0f);
        if (uniform_CameraUpAxis == 0x1) // X-up
        {
            coord = vec2(world_position.y, world_position.z);
        }
        else if (uniform_CameraUpAxis == 0x2) // Y-up
        {
            coord = vec2(world_position.x, world_position.z);
        }
        else if (uniform_CameraUpAxis == 0x4) // Z-up
        {
            coord = vec2(world_position.x, world_position.y);
        }
        else
        {
            discard;
        }

        // Minor grid spacing = 1/10th of major spacing
        const float minor_spacing = uniform_GridSpacing / 10.0f;

        const float minor     = grid(coord, minor_spacing);
        const float major     = grid(coord, uniform_GridSpacing);
        const float intensity = minor + major;

        const float distance_to_camera = length(world_position - fs_in.point_near);
        const float fade               = 1.0 - smoothstep(uniform_FadeStart, uniform_FadeEnd, distance_to_camera);

        const float alpha = intensity * fade;
        if (alpha < 0.01)
        {
            discard;
        }

        attachment_ColorOutput = vec4(uniform_ColorMinor.rgb * minor + uniform_ColorMajor.rgb * major, alpha);
    }
)";