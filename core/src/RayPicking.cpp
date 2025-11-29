#include <Core/Utils/RayPicking.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Core
{
    constexpr float kRayPlaneParallelEpsilon = 1e-4f;

    glm::f32vec2 MouseToNDC(const float mouse_x, const float mouse_y, const int32_t width, const int32_t height)
    {
        const auto f32_width  = static_cast<float>(width);
        const auto f32_height = static_cast<float>(height);

        const float x = (2.0f * mouse_x) / f32_width - 1.0f;
        const float y = 1.0f - (2.0f * mouse_y) / f32_height;

        return glm::f32vec2(x, y);
    }

    glm::vec4 NDCToEye(const glm::f32vec2& ndc, const glm::f32mat4x4& projection)
    {
        const glm::f32vec4 clip(ndc.x, ndc.y, -1.0f, 1.0f);
        const glm::f32vec4 eye = glm::inverse(projection) * clip;

        return glm::f32vec4(eye.x, eye.y, -1.0f, 0.0f);
    }

    glm::vec3 EyeToWorld(const glm::f32vec4& eye_ray, const glm::f32mat4x4& view)
    {
        const glm::f32vec3 world_ray = glm::f32vec3(glm::inverse(view) * eye_ray);

        return glm::normalize(world_ray);
    }

    Ray GeneratePickRay_PERSPECTIVE(const float mouse_x, const float mouse_y, const int32_t width, const int32_t height, const glm::f32mat4x4& view, const glm::f32mat4x4& projection, const glm::f32vec3& camera_pos)
    {
        const glm::f32vec2 ndc       = MouseToNDC(mouse_x, mouse_y, width, height);
        const glm::f32vec4 eye_ray   = NDCToEye(ndc, projection);
        const glm::f32vec3 world_ray = EyeToWorld(eye_ray, view);

        Ray ray;
        ray.origin    = camera_pos;
        ray.direction = world_ray;

        return ray;
    }

    Ray GeneratePickRay_ORTHOGRAPHIC(const float mouse_x, const float mouse_y, const int32_t width, const int32_t height, const glm::f32mat4x4& view, const glm::f32mat4x4& projection)
    {
        const auto f32_width  = static_cast<float>(width);
        const auto f32_height = static_cast<float>(height);

        const auto f32_mouse_screen = f32_height - mouse_y;

        const glm::f32vec4 viewport(0.0f, 0.0f, f32_width, f32_height);

        const glm::f32vec3 point_near = glm::unProject(
            glm::f32vec3(mouse_x, f32_mouse_screen, 0.0f),
            view,
            projection,
            viewport);

        const glm::f32vec3 point_far = glm::unProject(
            glm::f32vec3(mouse_x, f32_mouse_screen, 1.0f),
            view,
            projection,
            viewport);

        const auto direction = glm::normalize(point_far - point_near);

        Ray ray{};
        ray.origin    = point_near;
        ray.direction = direction;

        return ray;
    }

    bool RayPlaneIntersection(const Ray& ray, const glm::f32vec3& plane_point, const glm::f32vec3& plane_normal, glm::f32vec3& hit_point)
    {
        const auto denom = glm::dot(plane_normal, ray.direction);

        if (fabs(denom) < kRayPlaneParallelEpsilon)
        {
            return false;
        }

        const auto t = glm::dot(plane_point - ray.origin, plane_normal) / denom;

        if (t < 0)
        {
            return false;
        }

        hit_point = ray.origin + ray.direction * t;

        return true;
    }
}
