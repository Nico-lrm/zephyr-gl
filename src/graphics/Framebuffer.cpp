#include "Framebuffer.h"

#include <GL/glew.h>
#include <algorithm>
#include <utility>

namespace zephyr {

    Framebuffer::Framebuffer(const FramebufferCreateInfo& framebuffer_create_info) : Framebuffer()
    {
        if (framebuffer_create_info.color_attachments.empty() && !framebuffer_create_info.depth_attachment.has_value() && !framebuffer_create_info.stencil_attachment.has_value())
            throw std::runtime_error("Can't create an empty framebuffer");

        this->create_info = framebuffer_create_info;
        for (int i = 0; i < static_cast<int>(create_info.color_attachments.size()); i++) {
            this->images.emplace_back(framebuffer_create_info.color_attachments.at(i).texture_infos);
            glNamedFramebufferTexture(this->handle, GL_COLOR_ATTACHMENT0 + i, this->images.at(this->images.size() - 1).get_handle(), 0);
            this->attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
        }

        if (framebuffer_create_info.depth_attachment.has_value()) {
            this->images.emplace_back(framebuffer_create_info.depth_attachment->texture_infos);
            glNamedFramebufferTexture(this->handle, GL_DEPTH_ATTACHMENT, this->images.at(this->images.size() - 1).get_handle(), 0);
            this->attachments.emplace_back(GL_DEPTH_ATTACHMENT);
        }

        if (framebuffer_create_info.stencil_attachment.has_value()) {
            this->images.emplace_back(framebuffer_create_info.stencil_attachment->texture_infos);
            glNamedFramebufferTexture(this->handle, GL_STENCIL_ATTACHMENT, this->images.at(this->images.size() - 1).get_handle(), 0);
            this->attachments.emplace_back(GL_STENCIL_ATTACHMENT);
        }

        if (glCheckNamedFramebufferStatus(this->handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Can't finalize the actual bounded framebuffer.");
        }
    }

    Framebuffer::Framebuffer()
    {
        glCreateFramebuffers(1, &handle);
    }

    Framebuffer::~Framebuffer() noexcept
    {
        if (handle != 0)
            glDeleteFramebuffers(1, &handle);
    }

    Framebuffer::Framebuffer(Framebuffer&& old) noexcept
    {
        this->handle = std::exchange(old.handle, 0);
        this->images = std::exchange(old.images, {});
        this->attachments = std::exchange(old.attachments, {});
        this->create_info = std::exchange(old.create_info, {});
    }

    Framebuffer& Framebuffer::operator=(Framebuffer&& old) noexcept
    {
        if (&old == this)
            return *this;
        if (handle != 0)
            glDeleteFramebuffers(1, &handle);
        this->handle = std::exchange(old.handle, 0);
        this->images = std::exchange(old.images, {});
        this->attachments = std::exchange(old.attachments, {});
        this->create_info = std::exchange(old.create_info, {});
        return *this;
    }

    Image* Framebuffer::get_image(unsigned int index) const
    {
        if (this->images.empty())
            throw std::runtime_error("This framebuffer has no texture");
        const auto final_index{ std::clamp<unsigned int>(index, 0, static_cast<unsigned int>(images.size() - 1)) };
        return this->images.at(final_index).get();
    }

    const FramebufferCreateInfo& Framebuffer::get_attachments_infos() const noexcept
    {
        return this->create_info;
    }

} // zephyr