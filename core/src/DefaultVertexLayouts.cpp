#include <Core/Default/DefaultVertexLayouts.hpp>

#include <glad/glad.h>

#include <cstddef>

namespace Core
{
    namespace Default
    {
        template <>
        VertexBufferLayout GetVertexLayout<Point2D>()
        {
            return VertexBufferLayout{
                VertexBufferAttributeLayout{
                    .location   = 0,
                    .components = 2,
                    .type       = GL_FLOAT,
                    .normalize  = GL_FALSE,
                    .stride     = sizeof(Point2D),
                    .offset     = offsetof(Point2D, position)}};
        }

        template <>
        VertexBufferLayout GetVertexLayout<Point3D>()
        {
            return VertexBufferLayout{
                VertexBufferAttributeLayout{
                    .location   = 0,
                    .components = 3,
                    .type       = GL_FLOAT,
                    .normalize  = GL_FALSE,
                    .stride     = sizeof(Point3D),
                    .offset     = offsetof(Point3D, position)}};
        }

        template <>
        VertexBufferLayout GetVertexLayout<ColorPoint3D>()
        {
            return VertexBufferLayout{
                VertexBufferAttributeLayout{
                    .location   = 0,
                    .components = 3,
                    .type       = GL_FLOAT,
                    .normalize  = GL_FALSE,
                    .stride     = sizeof(ColorPoint3D),
                    .offset     = offsetof(ColorPoint3D, position)},
                VertexBufferAttributeLayout{
                    .location   = 1,
                    .components = 4,
                    .type       = GL_UNSIGNED_BYTE,
                    .normalize  = GL_TRUE,
                    .stride     = sizeof(ColorPoint3D),
                    .offset     = offsetof(ColorPoint3D, color)}};
        }
    }
}
