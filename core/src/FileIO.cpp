#include <Core/FileIO.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <spdlog/spdlog.h>

namespace Core
{
    bool LoadBinaryFile(const std::string& path, std::vector<uint8_t>& data)
    {
        if (std::ifstream file = std::ifstream(path, std::ios::binary | std::ios::ate))
        {
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            data.resize(static_cast<size_t>(size));

            return file.read(reinterpret_cast<char*>(data.data()), size).good();
        }

        spdlog::error("Failed to open for reading : {}", path);

        return false;
    }

    bool SaveBinaryFile(const std::string& path, const void* const data, const size_t size)
    {
        if (!data)
        {
            spdlog::error("Data for file {} is null!", path);
            return false;
        }

        if (!size)
        {
            spdlog::error("Data size for file {} is 0!", path);
            return false;
        }

        if (std::ofstream file = std::ofstream(path, std::ios::binary))
        {
            return file.write(reinterpret_cast<const char*>(data), size).good();
        }

        spdlog::error("Failed to open for writing : {}", path);

        return false;
    }

    bool LoadImage(const std::string& path, uint32_t& width, uint32_t& height, uint32_t& channels, std::vector<uint8_t>& data, const uint32_t force_channels)
    {
        int32_t w = 0;
        int32_t h = 0;
        int32_t c = 0;

        if (unsigned char* raw = stbi_load(path.c_str(), &w, &h, &c, force_channels))
        {
            width    = static_cast<uint32_t>(w);
            height   = static_cast<uint32_t>(h);
            channels = static_cast<uint32_t>(c);

            const size_t size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);

            data.assign(raw, raw + size);

            stbi_image_free(raw);

            return true;
        }

        return false;
    }
}
