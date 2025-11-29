#pragma once

#include <Core/Base/NonCopyableNonMovable.hpp>
#include <Core/OpenGL/Buffer.hpp>
#include <Core/Structures.hpp>

namespace Core
{
    class VertexArray
        : private NonCopyableNonMovable
    {
    private:
        struct VertexArrayIMPL;

    private:
        VertexArrayIMPL* _impl = nullptr;

    public:
        explicit VertexArray(Buffer* vertex_buffer, const bool vertex_buffer_ownership, Buffer* index_buffer, const bool index_buffer_ownership, const VertexBufferLayout& layout);
        ~VertexArray();

        [[nodiscard]] uint32_t GetID() const;

        void Bind();
        void Unbind();

        void DrawArray(const uint32_t mode, const uint32_t vertex_count);
        void DrawElements(const uint32_t mode, const uint32_t index_count, const uint32_t instance_count, const uint32_t base_instance);
    };
}
