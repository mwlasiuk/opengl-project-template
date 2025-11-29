#include "Buffer.hpp"

#include <glad/glad.h>

Buffer::Buffer(const uint32_t flags, const size_t size, const void* data)
    : _id(0), _size(size)
{
    glCreateBuffers(1, &_id);
    glNamedBufferStorage(_id, _size, data, flags);
}

Buffer::~Buffer()
{
    glInvalidateBufferData(_id);
    glDeleteBuffers(1, &_id);
}

void Buffer::Upload(const void* data, const size_t size) const
{
    glNamedBufferSubData(_id, 0, size, data);
}

void Buffer::SetAsShaderResource(const uint32_t resource_type, const uint32_t binding, const size_t size, const size_t offset) const
{
    glBindBufferRange(resource_type, binding, _id, offset, size);
}