#include "GLGraphicsDevice.h"

#include <GL/glew.h>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

void opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                             const GLchar* message, const void* userParam)
{
    std::string sourceName;
    std::string typeName;
    std::string severityName;

    switch(source)
    {
    case 0x8248:
        sourceName = "GL_DEBUG_SOURCE_SHADER_COMPILER_ARB";
        break;
    case 0x8246:
        sourceName = "GL_DEBUG_SOURCE_API_ARB";
        break;
    default:
        sourceName = "GL_DEBUG_SOURCE_UNKNOWN";
        break;
    }

    switch(type)
    {
    case 0x824c:
        typeName = "GL_DEBUG_TYPE_ERROR_ARB";
        break;
    case 0x8250:
        typeName = "GL_DEBUG_TYPE_PERFORMANCE_ARB";
        break;
    default:
        typeName = "GL_DEBUG_TYPE_UNKNOWN";
        break;
    }

    switch(severity)
    {
    case 0x9146:
        severityName = "GL_DEBUG_SEVERITY_HIGH_AMD";
        break;
    case 0x9147:
        severityName = "GL_DEBUG_SEVERITY_MEDIUM_AMD";
        break;
    case 0x9148:
        severityName = "GL_DEBUG_SEVERITY_LOW_AMD";
        break;
    case 0x826b:
        severityName = "GL_DEBUG_SEVERITY_NOTIFICATION";
        break;
    default:
        sourceName = "GL_DEBUG_SEVERITY_UNKNOWN";
        break;
    }
    const std::string finalString =
        "source : " + sourceName + "\ntype : " + typeName + "\nseverity : " + severityName + "\nmessage : " + message;
    std::cout << finalString << "\n";
    // throw std::runtime_error(finalString.c_str());
}

namespace zephyr
{
    GLGraphicsDevice::GLGraphicsDevice(const Window* window) : window_(dynamic_cast<const OpenGLWindow*>(window))
    {
        if(window_ == nullptr)
            throw std::runtime_error("Invalid window type");

        GLenum err{ glewInit() };
        if(GLEW_OK != err)
            throw std::runtime_error("Can't init glew");
        glDisable(GL_FRAMEBUFFER_SRGB);
#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(opengl_message_callback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

        // Check if required extensions and supported version are present
        assert(GLEW_VERSION_4_6 && "OpenGL 4.6 is not supported by this computer.");
    }
    GLGraphicsDevice::GLGraphicsDevice(GLGraphicsDevice&& other) noexcept
        : window_(std::exchange(other.window_, nullptr)),
          current_state_(std::exchange(other.current_state_, FrameState::READY)),
          state_tracker_(std::exchange(other.state_tracker_, GLStateTracker()))
    {}
    GLGraphicsDevice& GLGraphicsDevice::operator=(GLGraphicsDevice&& other) noexcept
    {
        if(this != &other)
        {
            window_ = std::exchange(other.window_, nullptr);
            current_state_ = std::exchange(other.current_state_, FrameState::READY);
            state_tracker_ = std::exchange(other.state_tracker_, GLStateTracker());
        }
        return *this;
    }
    void GLGraphicsDevice::begin_frame()
    {
        this->current_state_ = FrameState::RECORDING;
    }
    void GLGraphicsDevice::end_frame()
    {
        this->current_state_ = FrameState::END_RECORD;
    }
    void GLGraphicsDevice::submit()
    {
        this->current_state_ = FrameState::SUBMITTED;
    }
    void GLGraphicsDevice::present()
    {
        window_->swap_buffers();
        this->current_state_ = FrameState::READY;
    }
    void GLGraphicsDevice::wait_idle() const
    {
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
    void GLGraphicsDevice::resize_swapchain(std::optional<Extent2D> extent)
    {
        if(this->current_state_ != FrameState::READY)
            throw std::runtime_error("Cannot resize swapchain while frame is not ready");
        if(extent.has_value())
        {
            glViewport(0, 0, extent->width_, extent->height_);
        }
    }
    double GLGraphicsDevice::get_VRAM_usage() const noexcept
    {
        return 0.0;
    }
    FrameState GLGraphicsDevice::get_frame_state() const noexcept
    {
        return this->current_state_;
    }
} // namespace zephyr