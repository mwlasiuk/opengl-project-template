#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct CameraInputState
{
    bool is_first_mouse = true;

    double last_mouse_position_x = 0.0;
    double last_mouse_position_y = 0.0;

    bool mouse_pressed_left  = false;
    bool mouse_pressed_right = false;
};

class Camera
{
public:
    enum class UpAxis : int32_t
    {
        X = 0x1,
        Y = 0x2,
        Z = 0x4
    };

private:
    glm::vec3 _position        = {};
    glm::vec3 _target_position = {};
    glm::vec3 _up              = {};

    float _yaw      = {};
    float _pitch    = {};
    float _distance = {};

    UpAxis _up_axis = UpAxis::Y;

public:
    Camera();

    glm::mat4 GetViewMatrix() const;

    void Orbit(const float delta_x, const float delta_y);
    void Pan(const float delta_x, const float delta_y);
    void Zoom(const float offset);

    void SetUpAxis(const UpAxis axis);

    void ResetTarget();

    const glm::vec3& GetPosition() const { return _position; }
    const glm::vec3& GetTarget() const { return _target_position; }
    const glm::vec3& GetUp() const { return _up; }

    float GetYaw() const { return _yaw; }
    float GetPitch() const { return _pitch; }
    float GetDistance() const { return _distance; }

    UpAxis GetUpAxis() const { return _up_axis; }

private:
    void update_position();
};