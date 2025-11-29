#include <Core/Structures.hpp>

#include <type_traits>

namespace Core
{
    static_assert(sizeof(Point2D) == 8, "Point2D - invalid size!");
    static_assert(sizeof(Point3D) == 12, "Point3D - invalid size!");
    static_assert(sizeof(ColorPoint3D) == 16, "ColorPoint3D - invalid size!");
    static_assert(sizeof(SystemTransformUniforms) == 192, "SystemTransformUniforms - invalid size!");
    static_assert(sizeof(UpAxis) == 4, "UpAxis - invalid size!");

    static_assert(std::is_standard_layout_v<Point2D>, "Point2D - invalid memory layout!");
    static_assert(std::is_standard_layout_v<Point3D>, "Point3D - invalid memory layout!");
    static_assert(std::is_standard_layout_v<ColorPoint3D>, "ColorPoint3D - invalid memory layout!");
    static_assert(std::is_standard_layout_v<SystemTransformUniforms>, "SystemTransformUniforms - invalid memory layout!");
    static_assert(std::is_standard_layout_v<UpAxis>, "UpAxis - invalid memory layout!");
}
