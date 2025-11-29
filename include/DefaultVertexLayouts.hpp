#pragma once

// clang-format off
#include <glad/glad.h>
// clang-format on

#include "Structures.hpp"
#include "VertexBufferLayout.hpp"

namespace DefaultVertexLayouts
{
    template <typename T>
    static VertexBufferLayout GetVertexLayout();

    template <>
    static VertexBufferLayout GetVertexLayout<Point2D>()
    {
        return VertexBufferLayout{
            VertexBufferAttributeLayout{
                .location   = 0,
                .components = 2,
                .type       = GL_FLOAT,
                .normalize  = GL_FALSE,
                .stride     = sizeof(Point2D),
                .offset     = offsetof(Point2D, Point2D::position)}};
    }

    template <>
    static VertexBufferLayout GetVertexLayout<Point3D>()
    {
        return VertexBufferLayout{
            VertexBufferAttributeLayout{
                .location   = 0,
                .components = 3,
                .type       = GL_FLOAT,
                .normalize  = GL_FALSE,
                .stride     = sizeof(Point3D),
                .offset     = offsetof(Point3D, Point3D::position)}};
    }

    template <>
    static VertexBufferLayout GetVertexLayout<ColorPoint3D>()
    {
        return VertexBufferLayout{
            VertexBufferAttributeLayout{
                .location   = 0,
                .components = 3,
                .type       = GL_FLOAT,
                .normalize  = GL_FALSE,
                .stride     = sizeof(ColorPoint3D),
                .offset     = offsetof(ColorPoint3D, ColorPoint3D::position)},
            VertexBufferAttributeLayout{
                .location   = 1,
                .components = 4,
                .type       = GL_UNSIGNED_BYTE,
                .normalize  = GL_TRUE,
                .stride     = sizeof(ColorPoint3D),
                .offset     = offsetof(ColorPoint3D, ColorPoint3D::color)}};
    }
}
