#include <Core/SystemInfo.hpp>

#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>

// clang-format off
#if defined(_WIN32)
#include "Win32/SystemInfo.hpp"
#else
#include "Linux/SystemInfo.hpp"
#endif
// clang-format on

#include <glad/glad.h>

namespace Core
{
    // NOTE(m.wlasiuk) : more frequent queries can overwhelm OS and return invalid values
    constexpr auto kMinimalInfoSamplingInterval = std::chrono::nanoseconds(500'000'000);

    static void DetectGPUMemoryExtensions(bool& nvx, bool& ati)
    {
        int32_t count = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &count);

        for (int32_t i = 0; i < count; ++i)
        {
            const char* name = (const char*)glGetStringi(GL_EXTENSIONS, i);

            if (std::strcmp(name, "GL_NVX_gpu_memory_info") == 0)
            {
                nvx = true;
            }
            else if (std::strcmp(name, "GL_ATI_meminfo") == 0)
            {
                ati = true;
            }
        }
    }

    static GPUMemoryInfo::ATIPool ReadATIPool(uint32_t query)
    {
        GPUMemoryInfo::ATIPool pool{};

        int32_t mem[4] = {0};
        glGetIntegerv(query, mem);

        pool.free_mb                   = mem[0] / 1024;
        pool.largest_free_mb           = mem[1] / 1024;
        pool.total_auxiliary_free_mb   = mem[2] / 1024;
        pool.largest_auxiliary_free_mb = mem[3] / 1024;

        return pool;
    }

    static void FillNVX(GPUMemoryInfo& info)
    {
        int32_t dedicated     = 0;
        int32_t total         = 0;
        int32_t available     = 0;
        int32_t evictionCount = 0;
        int32_t evicted       = 0;

        glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicated);
        glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total);
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available);
        glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &evictionCount);
        glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &evicted);

        info.nvx_pool.dedicated_mb   = static_cast<uint32_t>(dedicated) / 1024U;
        info.nvx_pool.total_mb       = static_cast<uint32_t>(total) / 1024U;
        info.nvx_pool.available_mb   = static_cast<uint32_t>(available) / 1024U;
        info.nvx_pool.eviction_count = static_cast<uint32_t>(evictionCount);
        info.nvx_pool.evicted_mb     = static_cast<uint32_t>(evicted) / 1024U;
    }

    static void FillATI(GPUMemoryInfo& info)
    {
        info.texture      = ReadATIPool(GL_TEXTURE_FREE_MEMORY_ATI);
        info.vbo          = ReadATIPool(GL_VBO_FREE_MEMORY_ATI);
        info.renderbuffer = ReadATIPool(GL_RENDERBUFFER_FREE_MEMORY_ATI);
    }

    uint32_t GetSampleIntervalNS()
    {
        return static_cast<uint32_t>(kMinimalInfoSamplingInterval.count());
    }

    RAMInfo GetRAMInfo()
    {
        static RAMInfo cached_ram_info{};
        static auto    last_timestamp = std::chrono::steady_clock::now();

        auto current_timestamp = std::chrono::steady_clock::now();

        if (current_timestamp - last_timestamp < kMinimalInfoSamplingInterval)
        {
            return cached_ram_info;
        }

#if defined(_WIN32)
        cached_ram_info = GetRAMInfoWindows();
#else
        cached_ram_info = GetRAMInfoLinux();
#endif

        last_timestamp = current_timestamp;

        return cached_ram_info;
    }

    ProcessRAMInfo GetProcessRAMInfo()
    {
        static ProcessRAMInfo cached_process_info{};
        static auto           last_timestamp = std::chrono::steady_clock::now();

        auto current_timestamp = std::chrono::steady_clock::now();

        if (current_timestamp - last_timestamp < kMinimalInfoSamplingInterval)
        {
            return cached_process_info;
        }

#if defined(_WIN32)
        cached_process_info = GetProcessRAMInfoWindows();
#else
        cached_process_info = GetProcessRAMInfoLinux();
#endif

        last_timestamp = current_timestamp;

        return cached_process_info;
    }

    GPUInfo GetGPUInfo()
    {
        static GPUInfo cached_gpu_info{};

        static std::once_flag get_call_once_flag;

        std::call_once(get_call_once_flag,
                       [&]()
                       {
                           cached_gpu_info.gl_version                  = reinterpret_cast<const char*>(glGetString(GL_VERSION));
                           cached_gpu_info.gl_vendor                   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
                           cached_gpu_info.gl_renderer                 = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
                           cached_gpu_info.gl_shading_language_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
                       });

        return cached_gpu_info;
    }

    GPUMemoryInfo GetGPUMemoryInfo()
    {
        static GPUMemoryInfo cached{};
        static auto          last_timestamp = std::chrono::steady_clock::now();

        static std::once_flag detect_call_once_flag;
        static bool           has_memory_info_nvx = false;
        static bool           has_memory_info_ati = false;

        std::call_once(detect_call_once_flag,
                       []()
                       {
                           DetectGPUMemoryExtensions(has_memory_info_nvx, has_memory_info_ati);
                       });

        auto current_timestamp = std::chrono::steady_clock::now();

        if (current_timestamp - last_timestamp < kMinimalInfoSamplingInterval)
        {
            return cached;
        }

        cached = {};

        cached.has_memory_info_nvx = has_memory_info_nvx;
        cached.has_memory_info_ati = has_memory_info_ati;

        if (has_memory_info_nvx)
        {
            FillNVX(cached);
        }

        if (has_memory_info_ati)
        {
            FillATI(cached);
        }

        {
            last_timestamp = current_timestamp;
        }

        return cached;
    }
}
