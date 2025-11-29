#include <Core/OpenGL/Texture.hpp>

// clang-format off
#include <glad/glad.h>
// clang-format on

namespace Core
{
    struct Texture::TextureIMPL
    {
        uint32_t id = {};

        TextureSpecification specification = {};
    };
}

namespace Core
{
    Texture::Texture(const TextureSpecification& specification)
    {
        _impl = new TextureIMPL;

        _impl->id            = UINT32_MAX;
        _impl->specification = specification;

        glCreateTextures(GL_TEXTURE_2D, 1, &_impl->id);
        glTextureStorage2D(_impl->id, 1, _impl->specification.opengl_internal_format, _impl->specification.width, _impl->specification.height);

        SetParameterS32(GL_TEXTURE_MAG_FILTER, _impl->specification.sampler_parameters.opengl_texture_mag_filter);
        SetParameterS32(GL_TEXTURE_MIN_FILTER, _impl->specification.sampler_parameters.opengl_texture_min_filter);
        SetParameterS32(GL_TEXTURE_WRAP_S, _impl->specification.sampler_parameters.image_wrap_s);
        SetParameterS32(GL_TEXTURE_WRAP_T, _impl->specification.sampler_parameters.image_wrap_t);
        SetParameterS32(GL_TEXTURE_WRAP_R, _impl->specification.sampler_parameters.image_wrap_r);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &_impl->id);

        delete _impl;
    }

    uint32_t Texture::GetID() const
    {
        return _impl->id;
    }

    const TextureSpecification& Texture::GetTextureSpecification() const
    {
        return _impl->specification;
    }

    void Texture::Clear()
    {
        const uint32_t clear = 0x00000000;
        glClearTexImage(_impl->id, 0, _impl->specification.opengl_format, _impl->specification.opengl_type, &clear);
    }

    void Texture::UpLoadExternalImage(const void* const data)
    {
        glTextureSubImage2D(_impl->id, 0, 0, 0, _impl->specification.width, _impl->specification.height, _impl->specification.opengl_format, _impl->specification.opengl_type, data);
    }

    void Texture::SetParameterF32(const uint32_t parameter, const float value)
    {
        glTextureParameterf(_impl->id, parameter, value);
    }

    void Texture::SetParameterS32(const uint32_t parameter, const int32_t value)
    {
        glTextureParameteri(_impl->id, parameter, value);
    }
}
