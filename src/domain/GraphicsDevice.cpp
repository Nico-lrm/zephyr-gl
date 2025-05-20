#include "GraphicsDevice.h"

#include <iostream>
#include <utility>

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
    GraphicsDevice::GraphicsDevice(std::shared_ptr<Window> window) : window_(std::move(window))
    {
        const GLenum err{ glewInit() };
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

    GraphicsDevice::GraphicsDevice(GraphicsDevice&& other) noexcept
        : window_(std::exchange(other.window_, nullptr)),
          tracker_(std::exchange(other.tracker_, StateTracker()))
    {}

    GraphicsDevice& GraphicsDevice::operator=(GraphicsDevice&& other) noexcept
    {
        if(this != &other)
        {
            window_ = std::exchange(other.window_, nullptr);
            tracker_ = std::exchange(other.tracker_, StateTracker());
        }
        return *this;
    }

    std::shared_ptr<Buffer> GraphicsDevice::allocate_buffer(const BufferCreateInfo& create_info)
    {}

    std::shared_ptr<Buffer> GraphicsDevice::allocate_buffer(const ComposedBufferCreateInfo& create_info)
    {}

    std::shared_ptr<Buffer> GraphicsDevice::allocate_buffer(const AllocateBufferCreateInfo& allocate_buffer_create_info)
    {}

    void GraphicsDevice::allocate_sampler(std::string name, const SamplerCreateInfo& create_info)
    {}

    void GraphicsDevice::allocate_shader_module(std::string name, const ShaderCreateInfo& create_info)
    {}

    void GraphicsDevice::allocate_graphic_pipeline(std::string name, const GraphicPipelineCreateInfo& create_info)
    {}

    void GraphicsDevice::allocate_image(std::string name, const ImageCreateInfo& create_info)
    {}

    void GraphicsDevice::allocate_image(std::string name, const ImageLoadInfo& create_info)
    {}

    std::shared_ptr<Sampler> GraphicsDevice::get_sampler(std::string name)
    {}

    std::shared_ptr<ShaderModule> GraphicsDevice::get_shader_module(std::string name)
    {
        return this->shader_modules_[name] != nullptr ? this->shader_modules_[name] : nullptr;
    }

    std::shared_ptr<Pipeline> GraphicsDevice::get_graphic_pipeline(std::string name)
    {
        return this->pipelines_[name] != nullptr ? this->pipelines_[name] : nullptr;
    }

    std::shared_ptr<ImageView> GraphicsDevice::get_image(std::string name, const ImageViewCreateInfo& create_info)
    {}

    void GraphicsDevice::update_buffer(std::shared_ptr<Buffer> buffer, const BufferUpdateInfo& update_info)
    {}

    void GraphicsDevice::present() const
    {
        this->window_->swap_buffers();
    }

    void GraphicsDevice::wait_idle() const
    {
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
} // namespace zephyr