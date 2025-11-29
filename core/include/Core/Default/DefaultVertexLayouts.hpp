#pragma once

#include <Core/Structures.hpp>

namespace Core
{
    namespace Default
    {
        template <typename T>
        [[nodiscard]] VertexBufferLayout GetVertexLayout();

        template <>
        [[nodiscard]] VertexBufferLayout GetVertexLayout<Point2D>();

        template <>
        [[nodiscard]] VertexBufferLayout GetVertexLayout<Point3D>();

        template <>
        [[nodiscard]] VertexBufferLayout GetVertexLayout<ColorPoint3D>();
    }
}
