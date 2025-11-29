#pragma once

#include <windows.h>

#include <psapi.h>
#include <sysinfoapi.h>
#include <winioctl.h>

namespace Core
{
    static constexpr uint32_t kBytesToMegabytes = (1024U * 1024U);

    static RAMInfo GetRAMInfoWindows()
    {
        RAMInfo return_value{};

        SYSTEM_INFO system_info{};
        GetSystemInfo(&system_info);

        MEMORYSTATUSEX memory_status_ex{};
        memory_status_ex.dwLength = sizeof(memory_status_ex);
        GlobalMemoryStatusEx(&memory_status_ex);

        return_value.total_memory_mb = static_cast<uint32_t>(memory_status_ex.ullTotalPhys / kBytesToMegabytes);
        return_value.free_memory_mb  = static_cast<uint32_t>(memory_status_ex.ullAvailPhys / kBytesToMegabytes);
        return_value.used_memory_mb  = return_value.total_memory_mb - return_value.free_memory_mb;

        return return_value;
    }

    static ProcessRAMInfo GetProcessRAMInfoWindows()
    {
        ProcessRAMInfo return_value{};

        HANDLE process = GetCurrentProcess();

        PROCESS_MEMORY_COUNTERS_EX process_memory_counters_ex{};
        if (GetProcessMemoryInfo(process, (PROCESS_MEMORY_COUNTERS*)&process_memory_counters_ex, sizeof(process_memory_counters_ex)))
        {
            return_value.memory_used_mb    = static_cast<uint32_t>(process_memory_counters_ex.WorkingSetSize / kBytesToMegabytes);
            return_value.virtual_memory_mb = static_cast<uint32_t>(process_memory_counters_ex.PrivateUsage / kBytesToMegabytes);
        }

        return return_value;
    }
}
