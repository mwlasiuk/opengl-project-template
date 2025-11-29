#pragma once

#include <Core/Structures.hpp>

#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

namespace Core
{
    [[nodiscard]] bool LoadBinaryFile(const std::string& path, std::vector<uint8_t>& data);
    [[nodiscard]] bool SaveBinaryFile(const std::string& path, const void* const data, const size_t size);

    template <typename T>
    [[nodiscard]] bool LoadBinaryStructs(const std::string& path, std::vector<T>& out)
    {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable to be safely loaded from binary data!");
        static_assert(std::is_standard_layout_v<T>, "T should be standard-layout for predictable binary layout!");
        static_assert(!std::is_pointer_v<T>, "T must not be a pointer type!");
        static_assert(!std::is_reference_v<T>, "T must not be a reference type!");

        std::vector<uint8_t> raw = {};

        if (!LoadBinaryFile(path, raw))
        {
            return false;
        }

        if (raw.size() % sizeof(T) != 0)
        {
            return false;
        }

        const size_t count = raw.size() / sizeof(T);

        out.resize(count);

        std::memcpy(out.data(), raw.data(), raw.size());

        return true;
    }

    template <typename T>
    [[nodiscard]] bool SaveBinaryStructs(const std::string& path, const std::vector<T>& data)
    {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable to be safely written as binary!");
        static_assert(std::is_standard_layout_v<T>, "T must be standard-layout for predictable binary layout!");
        static_assert(!std::is_pointer_v<T>, "T must not be a pointer type!");
        static_assert(!std::is_reference_v<T>, "T must not be a reference type!");

        return SaveBinaryFile(path, data.data(), data.size() * sizeof(T));
    }

    [[nodiscard]] bool LoadImage(const std::string& path, uint32_t& width, uint32_t& height, uint32_t& channels, std::vector<uint8_t>& data, const uint32_t force_channels = 0);
}