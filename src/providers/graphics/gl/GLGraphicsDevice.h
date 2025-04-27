#pragma once

#include "../../windows/OpenGLWindow.h"

#include <optional>

namespace zephyr
{
    struct GLStateTracker
    {
        Viewport viewport{};
    };

    class GLGraphicsDevice
    {
      public:
        explicit GLGraphicsDevice(const Window* window);
        ~GLGraphicsDevice() = default;
        GLGraphicsDevice(GLGraphicsDevice&& other) noexcept;
        GLGraphicsDevice& operator=(GLGraphicsDevice&& other) noexcept;
        GLGraphicsDevice(const GLGraphicsDevice&) = delete;
        GLGraphicsDevice& operator=(const GLGraphicsDevice&) = delete;

        void begin_frame();
        void end_frame();
        void submit();
        void present();
        void wait_idle() const;
        void resize_swapchain(std::optional<Extent2D> extent);
        [[nodiscard]] double get_VRAM_usage() const noexcept;
        [[nodiscard]] FrameState get_frame_state() const noexcept;

      private:
        const OpenGLWindow* window_;
        FrameState current_state_{ FrameState::READY };
        GLStateTracker state_tracker_{};
    };

} // namespace zephyr