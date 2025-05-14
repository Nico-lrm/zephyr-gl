#pragma once

#include "../../graphics/Types.h"

#include <GLFW/glfw3.h>

namespace zephyr
{
    class GLFWOpenGLWindow
    {
      public:
        GLFWOpenGLWindow();
        ~GLFWOpenGLWindow();
        GLFWOpenGLWindow(GLFWOpenGLWindow&& other) noexcept;
        GLFWOpenGLWindow& operator=(GLFWOpenGLWindow&& other) noexcept;
        GLFWOpenGLWindow(const GLFWOpenGLWindow&) = delete;
        GLFWOpenGLWindow& operator=(const GLFWOpenGLWindow&) = delete;

        void process_events();
        void swap_buffers() const;
        [[nodiscard]] bool should_close() const;
        [[nodiscard]] const void* get_handle() const;
        [[nodiscard]] Extent2D get_window_extent() const;

      private:
        int last_width_{}, last_height_{};
        GLFWwindow* window_{ nullptr };
        GLFWmonitor* monitor_{ nullptr };
    };
}; // namespace zephyr
