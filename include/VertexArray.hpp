#pragma once

#include "Buffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
private:
    uint32_t _id = 0;

    Buffer* _vbo           = nullptr;
    bool    _vbo_ownership = false;

    Buffer* _ibo           = nullptr;
    bool    _ibo_ownership = false;

public:
    VertexArray(Buffer* vertex_buffer, const bool vertex_buffer_ownership, Buffer* index_buffer, const bool index_buffer_ownership, const VertexBufferLayout& layout);
    ~VertexArray();

    uint32_t GetID() const { return _id; }

    void Bind();
    void Unbind();

    void DrawArray(const uint32_t mode, const uint32_t vertex_count);
    void DrawElements(const uint32_t mode, const uint32_t index_count, const uint32_t instance_count, const uint32_t base_instance);
};