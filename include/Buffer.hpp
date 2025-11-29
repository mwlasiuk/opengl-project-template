#pragma once

#include <cstdint>

class Buffer
{
private:
    uint32_t _id   = 0;
    size_t   _size = 0;

public:
    Buffer(const uint32_t flags, const size_t size, const void* data = nullptr);
    ~Buffer();

    uint32_t GetID() const { return _id; }
    size_t   GetSize() const { return _size; }

    void Upload(const void* data, const size_t size) const;
    void SetAsShaderResource(const uint32_t resource_type, const uint32_t binding, const size_t size, const size_t offset) const;
};
