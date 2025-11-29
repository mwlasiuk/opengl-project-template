#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct ShaderDescriptor
{
    uint32_t    shader_type = 0;
    int32_t     source_size = 0;
    const char* source      = nullptr;
};

typedef std::vector<ShaderDescriptor> ProgramDescriptor;

class Program
{
private:
    uint32_t                                 _id                     = 0;
    std::unordered_map<std::string, int32_t> _uniform_location_cache = {};

public:
    Program(const ProgramDescriptor& program_descriptor);
    ~Program();

    uint32_t GetID() const { return _id; }

    void Bind();
    void Unbind();

    void PushUniformSamplerUnit(const std::string& name, const uint32_t unit, const uint32_t texture);

    void PushUniformS32(const std::string& name, const int32_t value);
    void PushUniformU32(const std::string& name, const uint32_t value);
    void PushUniform1F32(const std::string& name, const float value);
    void PushUniform2F32(const std::string& name, const glm::vec2& value);
    void PushUniform3F32(const std::string& name, const glm::vec3& value);
    void PushUniform4F32(const std::string& name, const glm::vec4& value);
    void PushUniform16F32(const std::string& name, const glm::mat4& value);

    int32_t GetUniformLocation(const std::string& name);
};