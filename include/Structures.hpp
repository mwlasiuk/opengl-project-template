#pragma once

#include <glm/glm.hpp>

struct Point2D
{
    glm::vec<2, float> position;
};

struct Point3D
{
    glm::vec<3, float> position;
};

struct ColorPoint3D
{
    glm::vec<3, float>   position;
    glm::vec<4, uint8_t> color;
};

struct SystemTransformUniforms
{
    glm::mat4 preview_projection;
    glm::mat4 preview_view;
};
