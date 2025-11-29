#include "VertexArray.hpp"

#include <glad/glad.h>

VertexArray::VertexArray(Buffer* vertex_buffer, const bool vertex_buffer_ownership, Buffer* index_buffer, const bool index_buffer_ownership, const VertexBufferLayout& layout)
    : _vbo(vertex_buffer),
      _vbo_ownership(vertex_buffer_ownership),
      _ibo(index_buffer),
      _ibo_ownership(index_buffer_ownership)
{
    glCreateVertexArrays(1, &_id);

    if (_ibo)
    {
        glVertexArrayElementBuffer(_id, _ibo->GetID());
    }

    if (!_vbo)
    {
        return;
    }

    for (const VertexBufferAttributeLayout& attribute_layout : layout)
    {
        const auto& [location, components, type, normalize, stride, offset] = attribute_layout;

        glVertexArrayVertexBuffer(_id, location, _vbo->GetID(), offset, stride);
        glEnableVertexArrayAttrib(_id, location);
        glVertexArrayAttribFormat(_id, location, components, type, normalize, 0);
        glVertexArrayAttribBinding(_id, location, location);
    }
}

VertexArray::~VertexArray()
{
    if (_ibo_ownership && _ibo)
    {
        delete _ibo;
    }

    if (_vbo_ownership && _vbo)
    {
        delete _vbo;
    }

    glDeleteVertexArrays(1, &_id);
}

void VertexArray::Bind()
{
    glBindVertexArray(_id);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::DrawArray(const uint32_t mode, const uint32_t vertex_count)
{
    glDrawArrays(mode, 0, vertex_count);
}

void VertexArray::DrawElements(const uint32_t mode, const uint32_t index_count, const uint32_t instance_count, const uint32_t base_instance)
{
    glDrawElementsInstancedBaseVertexBaseInstance(mode, index_count, GL_UNSIGNED_INT, nullptr, instance_count, 0, base_instance);
}