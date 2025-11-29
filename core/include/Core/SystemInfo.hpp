#pragma once

#include <Core/Structures.hpp>

namespace Core
{
    [[nodiscard]] uint32_t       GetSampleIntervalNS();
    [[nodiscard]] RAMInfo        GetRAMInfo();
    [[nodiscard]] ProcessRAMInfo GetProcessRAMInfo();
    [[nodiscard]] GPUInfo        GetGPUInfo();
    [[nodiscard]] GPUMemoryInfo  GetGPUMemoryInfo();
}
