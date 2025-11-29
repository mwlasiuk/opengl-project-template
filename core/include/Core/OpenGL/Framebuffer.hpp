#pragma once

#include <Core/OpenGL/Texture.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Core
{
    class Framebuffer
    {
    private:
        struct FramebufferIMPL;

    private:
        FramebufferIMPL* _impl = nullptr;

    public:
        explicit Framebuffer(const std::vector<Texture*>& color_attachments, Texture* depth_attachment);
        ~Framebuffer();

        [[nodiscard]] uint32_t GetID() const;

        void Clear(const glm::f32vec4& clear_color = glm::zero<glm::f32vec4>(), const glm::f32vec4& clear_depth = glm::one<glm::f32vec4>());
        void Bind();
        void UnBind();
        void BlitTo(Framebuffer* destination);
    };
}
