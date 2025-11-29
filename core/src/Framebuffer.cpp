#include <Core/OpenGL/Framebuffer.hpp>

#include <spdlog/spdlog.h>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <array>

namespace Core
{
    struct Framebuffer::FramebufferIMPL
    {
        uint32_t id = {};

        std::vector<Texture*> color_attachments = {};
        Texture*              depth_attachment  = {};
    };
}

namespace Core
{
    Framebuffer::Framebuffer(const std::vector<Texture*>& color_attachments, Texture* depth_attachment)
    {
        _impl = new FramebufferIMPL;

        _impl->id                = UINT32_MAX;
        _impl->color_attachments = color_attachments;
        _impl->depth_attachment  = depth_attachment;

        glCreateFramebuffers(1, &_impl->id);

        for (size_t i = 0; i < _impl->color_attachments.size(); i++)
        {
            const auto attachment_id = static_cast<uint32_t>(GL_COLOR_ATTACHMENT0 + i);

            glNamedFramebufferTexture(_impl->id, attachment_id, _impl->color_attachments[i]->GetID(), 0);
        }

        if (_impl->depth_attachment)
        {
            glNamedFramebufferTexture(_impl->id, GL_DEPTH_ATTACHMENT, _impl->depth_attachment->GetID(), 0);
        }

        if (glCheckNamedFramebufferStatus(_impl->id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            spdlog::error("Framebuffer ID = {} - incomplete!", _impl->id);
        }

        glNamedFramebufferReadBuffer(_impl->id, GL_NONE);
        glNamedFramebufferDrawBuffer(_impl->id, GL_NONE);
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &_impl->id);

        delete _impl;
    }

    uint32_t Framebuffer::GetID() const
    {
        return _impl->id;
    }

    void Framebuffer::Clear(const glm::f32vec4& clear_color, const glm::f32vec4& clear_depth)
    {
        if (_impl->depth_attachment)
        {
            glClearNamedFramebufferfv(_impl->id, GL_DEPTH, 0, glm::value_ptr(clear_depth));
        }

        for (size_t i = 0; i < _impl->color_attachments.size(); i++)
        {
            const auto attachment_index = static_cast<uint32_t>(i);

            glClearNamedFramebufferfv(_impl->id, GL_COLOR, attachment_index, glm::value_ptr(clear_color));
        }
    }

    void Framebuffer::Bind()
    {
        if (_impl->color_attachments.size() > 0)
        {
            if (_impl->color_attachments.size() == 1)
            {
                glNamedFramebufferReadBuffer(_impl->id, GL_COLOR_ATTACHMENT0);
                glNamedFramebufferDrawBuffer(_impl->id, GL_COLOR_ATTACHMENT0);
            }
            else
            {
                constexpr auto kColorAttachmentIDs = []
                {
                    std::array<uint32_t, 32> color_attachment{};

                    for (size_t i = 0; i < color_attachment.size(); ++i)
                    {
                        color_attachment[i] = static_cast<uint32_t>(GL_COLOR_ATTACHMENT0) + static_cast<uint32_t>(i);
                    }

                    return color_attachment;
                }();

                const auto  color_attachment_count = static_cast<int32_t>(_impl->color_attachments.size());
                const auto* color_attachments      = kColorAttachmentIDs.data();

                glNamedFramebufferReadBuffer(_impl->id, GL_COLOR_ATTACHMENT0);
                glNamedFramebufferDrawBuffers(_impl->id, color_attachment_count, color_attachments);
            }
        }
        else
        {
            glNamedFramebufferReadBuffer(_impl->id, GL_NONE);
            glNamedFramebufferDrawBuffer(_impl->id, GL_NONE);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, _impl->id);
    }

    void Framebuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::BlitTo(Framebuffer* destination)
    {
        const auto color_attachment_count_differs    = _impl->color_attachments.size() != destination->_impl->color_attachments.size();
        const auto depth_attachment_presence_differs = (_impl->depth_attachment && !destination->_impl->depth_attachment) || (!_impl->depth_attachment && destination->_impl->depth_attachment);

        if (color_attachment_count_differs)
        {
            spdlog::error("Framebuffer ID = {} blit to ID = {} - color attachment count does not match - {} vs {}!",
                          _impl->id, destination->_impl->id,
                          _impl->color_attachments.size(), destination->_impl->color_attachments.size());
            return;
        }

        if (depth_attachment_presence_differs)
        {
            spdlog::error("Framebuffer ID = {} blit to ID = {} - color attachment count does not match - {} vs {}!",
                          _impl->id, destination->_impl->id,
                          _impl->depth_attachment ? 1 : 0, destination->_impl->depth_attachment ? 1 : 0);
            return;
        }

        for (size_t i = 0; i < _impl->color_attachments.size(); i++)
        {
            const auto attachment_id = static_cast<uint32_t>(GL_COLOR_ATTACHMENT0 + i);

            const uint32_t source_width  = _impl->color_attachments[i]->GetTextureSpecification().width;
            const uint32_t source_height = _impl->color_attachments[i]->GetTextureSpecification().height;

            const uint32_t destination_width  = destination->_impl->color_attachments[i]->GetTextureSpecification().width;
            const uint32_t destination_height = destination->_impl->color_attachments[i]->GetTextureSpecification().height;

            glNamedFramebufferReadBuffer(_impl->id, attachment_id);
            glNamedFramebufferDrawBuffer(destination->_impl->id, attachment_id);

            glBlitNamedFramebuffer(_impl->id, destination->_impl->id,
                                   0, 0, source_width, source_height,
                                   0, 0, destination_width, destination_height,
                                   GL_COLOR_BUFFER_BIT, GL_LINEAR);
        }

        if (_impl->depth_attachment && destination->_impl->depth_attachment)
        {
            const uint32_t source_width  = _impl->depth_attachment->GetTextureSpecification().width;
            const uint32_t source_height = _impl->depth_attachment->GetTextureSpecification().height;

            const uint32_t destination_width  = destination->_impl->depth_attachment->GetTextureSpecification().width;
            const uint32_t destination_height = destination->_impl->depth_attachment->GetTextureSpecification().height;

            glNamedFramebufferReadBuffer(_impl->id, GL_COLOR_ATTACHMENT0);
            glNamedFramebufferDrawBuffer(destination->_impl->id, GL_COLOR_ATTACHMENT0);

            glBlitNamedFramebuffer(_impl->id, destination->_impl->id,
                                   0, 0, source_width, source_height,
                                   0, 0, destination_width, destination_height,
                                   GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        }
    }
}
