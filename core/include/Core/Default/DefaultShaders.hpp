#pragma once

#include <cstddef>
#include <cstdint>

namespace Core
{
    namespace Default
    {
        [[nodiscard]] const char* ColorPointVertexCode();
        [[nodiscard]] size_t      ColorPointVertexSize();
        [[nodiscard]] const char* ColorPointFragmentCode();
        [[nodiscard]] size_t      ColorPointFragmentSize();

        [[nodiscard]] const char* PointVertexCode();
        [[nodiscard]] size_t      PointVertexSize();
        [[nodiscard]] const char* PointFragmentCode();
        [[nodiscard]] size_t      PointFragmentSize();

        [[nodiscard]] const char* GridVertexCode();
        [[nodiscard]] size_t      GridVertexSize();
        [[nodiscard]] const char* GridFragmentCode();
        [[nodiscard]] size_t      GridFragmentSize();
    }
}
