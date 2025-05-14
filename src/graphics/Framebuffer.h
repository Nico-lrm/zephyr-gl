#pragma once

#include "Image.h"
#include "Types.h"
#include "Utils.h"

#include <memory>
#include <optional>
#include <vector>

namespace zephyr
{

    struct ColorAttachment
    {
        ImageCreateInfo texture_infos{};
        AttachmentLoadOperation attachment_load_operation{ AttachmentLoadOperation::CLEAR };
        float clear_value[4]{ 0.f };
    };

    struct DepthStencilAttachment
    {
        ImageCreateInfo texture_infos{};
        AttachmentLoadOperation attachment_load_operation{ AttachmentLoadOperation::CLEAR };
        float depth_clear_value{ 1.f };
        int stencil_clear_value{};
    };

    struct FramebufferCreateInfo
    {
        std::vector<ColorAttachment> color_attachments;
        std::optional<DepthStencilAttachment> depth_attachment{};
        std::optional<DepthStencilAttachment> stencil_attachment{};
    };

    class Framebuffer : public GLObject<GLuint>
    {
      public:
        ~Framebuffer() noexcept;
        Framebuffer(Framebuffer&& old) noexcept;
        Framebuffer& operator=(Framebuffer&& old) noexcept;
        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;
        [[nodiscard]] Image* get_image(unsigned int index) const;
        [[nodiscard]] const FramebufferCreateInfo& get_attachments_infos() const noexcept;

      private:
        explicit Framebuffer(const FramebufferCreateInfo& framebuffer_create_info);
        Framebuffer();
        FramebufferCreateInfo create_info;
        std::vector<GLenum> attachments;
        std::vector<std::shared_ptr<Image>> images;
    };

    struct FramebufferBlitInfo
    {
        Framebuffer* source{ nullptr };
        Framebuffer* destination{ nullptr };
        Extent2D source_extent{};
        Extent2D destination_extent{};
        Offset2D source_start_coordinate{};
        Offset2D destination_start_coordinate{};
        BufferBit buffer_bit{ BufferBit::COLOR };
        Filter texture_filter{ Filter::LINEAR };
    };

} // namespace zephyr
