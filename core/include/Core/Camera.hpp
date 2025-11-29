#pragma once

#include <Core/Structures.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
    class Camera
    {
    private:
        glm::f32vec3 _position        = {};
        glm::f32vec3 _target_position = {};
        glm::f32vec3 _up              = {};

        float _yaw      = {};
        float _pitch    = {};
        float _distance = {};

        UpAxis _up_axis = UpAxis::DEFAULT;

    public:
        Camera();

        void Orbit(const float delta_x, const float delta_y);
        void Pan(const float delta_x, const float delta_y);
        void Zoom(const float offset);

        void SetUpAxis(const UpAxis axis);

        void ResetTarget();

        [[nodiscard]] glm::f32mat4x4 GetViewMatrix() const noexcept;

        [[nodiscard]] const glm::f32vec3& GetPosition() const noexcept { return _position; }
        [[nodiscard]] const glm::f32vec3& GetTarget() const noexcept { return _target_position; }
        [[nodiscard]] const glm::f32vec3& GetUp() const noexcept { return _up; }

        [[nodiscard]] float GetYaw() const noexcept { return _yaw; }
        [[nodiscard]] float GetPitch() const noexcept { return _pitch; }
        [[nodiscard]] float GetDistance() const noexcept { return _distance; }

        [[nodiscard]] UpAxis GetUpAxis() const noexcept { return _up_axis; }

    private:
        void update_position_from_spherical();
    };
}
