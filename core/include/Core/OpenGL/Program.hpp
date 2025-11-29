#pragma once

#include <Core/Base/NonCopyableNonMovable.hpp>
#include <Core/Structures.hpp>

#include <string>

namespace Core
{
    class Program
        : private NonCopyableNonMovable
    {
    private:
        struct ProgramIMPL;

    private:
        ProgramIMPL* _impl = nullptr;

    public:
        explicit Program(const ProgramDescriptor& program_descriptor);
        ~Program();

        [[nodiscard]] uint32_t GetID() const;

        void Bind();
        void Unbind();

        void PushUniformSamplerUnit(const uint32_t unit, const uint32_t texture);

        void PushUniformS32(const std::string& name, const int32_t value);
        void PushUniformU32(const std::string& name, const uint32_t value);
        void PushUniform1F32(const std::string& name, const float value);
        void PushUniform2F32(const std::string& name, const glm::f32vec2& value);
        void PushUniform3F32(const std::string& name, const glm::f32vec3& value);
        void PushUniform4F32(const std::string& name, const glm::f32vec4& value);
        void PushUniform16F32(const std::string& name, const glm::f32mat4x4& value);

        int32_t GetUniformLocation(const std::string& name);
    };
}