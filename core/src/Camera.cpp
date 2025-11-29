#include <Core/Camera.hpp>

namespace Core
{
    static constexpr float kPitchLimitFromPerpendicularDegrees = 0.5f;

    static constexpr float kYawLimitDegrees   = glm::degrees(glm::two_pi<float>());
    static constexpr float kPitchLimitDegrees = glm::degrees(glm::half_pi<float>() - glm::radians(kPitchLimitFromPerpendicularDegrees));

    static constexpr float kMinimalZoomDistance = 1.0f;

    static constexpr float kCameraSpeedRotation = 0.15f;
    static constexpr float kCameraSpeedPan      = 0.01f;
    static constexpr float kCameraSpeedZoom     = 0.5f;

    Camera::Camera()
        : _position(0.0f, -1.0f, 0.0f),
          _target_position(0.0f, 0.0f, 0.0f),
          _yaw(0.0f),
          _pitch(0.0f),
          _distance(1),
          _up_axis(UpAxis::Y)
    {
        SetUpAxis(UpAxis::Y);
        update_position_from_spherical();
    }

    void Camera::Orbit(const float delta_x, const float delta_y)
    {
        _yaw += delta_x * kCameraSpeedRotation;
        _pitch -= delta_y * kCameraSpeedRotation;

        _yaw   = glm::mod(_yaw, kYawLimitDegrees);
        _pitch = glm::clamp(_pitch, -kPitchLimitDegrees, kPitchLimitDegrees);

        update_position_from_spherical();
    }

    void Camera::Pan(const float delta_x, const float delta_y)
    {
        const glm::f32vec3 forward_direction = glm::normalize(_target_position - _position);
        const glm::f32vec3 right_direction   = glm::normalize(glm::cross(forward_direction, _up));
        const glm::f32vec3 up_direction      = glm::cross(right_direction, forward_direction);

        const glm::f32vec3 right_shift = right_direction * delta_x;
        const glm::f32vec3 up_shift    = up_direction * delta_y;

        const glm::f32vec3 shift = -1.0f * (right_shift + up_shift) * kCameraSpeedPan;

        _target_position += shift;
        _position += shift;
    }

    void Camera::Zoom(const float offset)
    {
        _distance -= offset * kCameraSpeedZoom;

        if (_distance < kMinimalZoomDistance)
        {
            _distance = kMinimalZoomDistance;
        }

        update_position_from_spherical();
    }

    void Camera::SetUpAxis(const UpAxis axis)
    {
        _up_axis = axis;
        _up      = UpAxisToGLM(_up_axis);

        update_position_from_spherical();
    }

    void Camera::ResetTarget()
    {
        _position -= _target_position;
        _target_position = {};
    }

    glm::f32mat4x4 Camera::GetViewMatrix() const noexcept
    {
        return glm::lookAt(_position, _target_position, _up);
    }

    void Camera::update_position_from_spherical()
    {
        // Fix reversed rotation when Y is up
        const auto yaw_sign = (_up_axis == UpAxis::Y) ? -1.0f : 1.0f;

        const float pitch_radians = glm::radians(_pitch);
        const float yaw_radians   = glm::radians(_yaw) * yaw_sign;

        const float pitch_sin = glm::sin(pitch_radians);
        const float pitch_cos = glm::cos(pitch_radians);

        const float yaw_sin = glm::sin(yaw_radians);
        const float yaw_cos = glm::cos(yaw_radians);

        const float a = _distance * pitch_cos * yaw_cos;
        const float b = _distance * pitch_cos * yaw_sin;
        const float c = _distance * pitch_sin;

        glm::f32vec3 offset = {0.0f, 0.0f, 0.0f};

        switch (_up_axis)
        {
        case UpAxis::Y:
            {
                offset = {a, c, b};
                break;
            }

        case UpAxis::Z:
            {
                offset = {a, b, c};
                break;
            }

        case UpAxis::X:
            {
                offset = {c, a, b};
                break;
            }
        }

        _position = _target_position + offset;
    }
}
