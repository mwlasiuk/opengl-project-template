#pragma once

#include <mntent.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <unistd.h>

namespace Core
{
    static constexpr uint32_t kBytesToMegabytes = (1024U * 1024U);

    static RAMInfo GetRAMInfoLinux()
    {
        RAMInfo return_value{};

        struct sysinfo sys;
        if (sysinfo(&sys) != 0)
        {
            return return_value;
        }

        return_value.total_memory_mb = static_cast<uint32_t>(sys.totalram * sys.mem_unit / kBytesToMegabytes);
        return_value.free_memory_mb  = static_cast<uint32_t>(sys.freeram * sys.mem_unit / kBytesToMegabytes);
        return_value.used_memory_mb  = return_value.total_memory_mb - return_value.free_memory_mb;

        return return_value;
    }

    static ProcessRAMInfo GetProcessRAMInfoLinux()
    {
        ProcessRAMInfo return_value{};

        long page_size = sysconf(_SC_PAGESIZE);

        std::ifstream statm("/proc/self/statm");
        long          size     = 0;
        long          resident = 0;
        statm >> size >> resident;

        return_value.virtual_memory_mb = static_cast<uint32_t>(size * page_size / kBytesToMegabytes);
        return_value.memory_used_mb    = static_cast<uint32_t>(resident * page_size / kBytesToMegabytes);

        std::ifstream stat("/proc/self/stat");

        std::string tmp;
        for (int32_t i = 0; i < 13; i++)
        {
            stat >> tmp;
        }

        return return_value;
    }
}
