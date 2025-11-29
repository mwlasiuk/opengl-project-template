#pragma once

#include <Core/Structures.hpp>

namespace Core
{
    [[nodiscard]] glm::f32vec2 MouseToNDC(const float mouse_x, const float mouse_y, const int32_t width, const int32_t height);
    [[nodiscard]] glm::f32vec4 NDCToEye(const glm::f32vec2& ndc, const glm::f32mat4x4& projection);
    [[nodiscard]] glm::f32vec3 EyeToWorld(const glm::f32vec4& eye_ray, const glm::f32mat4x4& view);
    [[nodiscard]] Ray          GeneratePickRay_PERSPECTIVE(const float mouse_x, const float mouse_y, const int32_t width, const int32_t height, const glm::f32mat4x4& view, const glm::f32mat4x4& projection, const glm::f32vec3& camera_pos);
    [[nodiscard]] Ray          GeneratePickRay_ORTHOGRAPHIC(const float mouse_x, const float mouse_y, const int32_t width, const int32_t height, const glm::f32mat4x4& view, const glm::f32mat4x4& projection);
    [[nodiscard]] bool         RayPlaneIntersection(const Ray& ray, const glm::f32vec3& plane_point, const glm::f32vec3& plane_normal, glm::f32vec3& hit_point);
}
