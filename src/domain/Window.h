#pragma once

#include <GLFW/glfw3.h>
#include "../graphics/Types.h"

namespace zephyr
{
    class Window
    {
      public:
        Window();
        ~Window() noexcept;
        Window(Window&& other) noexcept;
        Window& operator=(Window&& other) noexcept;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        // bool get_key()...
        void swap_buffers() const;
        [[nodiscard]] bool should_close() const;
        [[nodiscard]] Extent2D get_window_extent() const;
      private:
        int last_width_{}, last_height_{};
        GLFWwindow* window_;
        GLFWmonitor* monitor_;
    };
} // namespace zephyr
