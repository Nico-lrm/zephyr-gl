#include "GLFWOpenGLWindow.h"

#include <iostream>
#include <utility>

void error_callback([[maybe_unused]] int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

namespace zephyr
{
    GLFWOpenGLWindow::GLFWOpenGLWindow()
    {
        if(!glfwInit())
            throw std::runtime_error("Failed to initialize GLFW");
        glfwSetErrorCallback(error_callback);

        this->monitor_ = glfwGetPrimaryMonitor();
        if(!monitor_)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to get a primary monitor");
        }

        const GLFWvidmode* mode = glfwGetVideoMode(monitor_);
        if(!mode)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to get video mode");
        }

        const int width = static_cast<int>(mode->width * 0.9);
        const int height = static_cast<int>(mode->height * 0.9);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_FALSE);

        window_ = glfwCreateWindow(width, height, APP_NAME.data(), nullptr, nullptr);
        if(!window_)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window_);
    }

    GLFWOpenGLWindow::~GLFWOpenGLWindow()
    {
        if(window_)
        {
            glfwDestroyWindow(window_);
            glfwTerminate();
        }
    }

    GLFWOpenGLWindow::GLFWOpenGLWindow(GLFWOpenGLWindow&& other) noexcept
        : last_width_(std::exchange(other.last_width_, 0)), last_height_(std::exchange(other.last_height_, 0)),
          window_(std::exchange(other.window_, nullptr)), monitor_(std::exchange(other.monitor_, nullptr))
    {}

    GLFWOpenGLWindow& GLFWOpenGLWindow::operator=(GLFWOpenGLWindow&& other) noexcept
    {
        if(this != &other)
        {
            if(window_)
            {
                glfwDestroyWindow(window_);
            }
            window_ = std::exchange(other.window_, nullptr);
            monitor_ = std::exchange(other.monitor_, nullptr);
            last_width_ = std::exchange(other.last_width_, 0);
            last_height_ = std::exchange(other.last_height_, 0);
        }
        return *this;
    }

    void GLFWOpenGLWindow::process_events()
    {
        glfwPollEvents();
    }

    void GLFWOpenGLWindow::swap_buffers() const
    {
        glfwSwapBuffers(window_);
    }

    bool GLFWOpenGLWindow::should_close() const
    {
        return glfwWindowShouldClose(this->window_);
    }

    const void* GLFWOpenGLWindow::get_handle() const
    {
        return this->window_;
    }

    Extent2D GLFWOpenGLWindow::get_window_extent() const
    {
        int width{}, height{};
        glfwGetFramebufferSize(window_, &width, &height);
        return Extent2D{ width, height };
    }
} // namespace zephyr
