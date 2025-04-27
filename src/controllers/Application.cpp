#include "Application.h"

#include "../providers/graphics/gl/GLGraphicsDevice.h"
#include "../providers/windows/GLFWOpenGLWindow.h"
#include "../providers/windows/OpenGLWindow.h"

namespace zephyr
{
    Application::Application(const WindowType window_type, const RenderAPI render_api)
    {
        switch(window_type)
        {
        case WindowType::GLFW:
            switch(render_api)
            {
            case RenderAPI::OpenGL:
                this->window_ = std::make_unique<OpenGLWindow>(std::make_shared<GLFWOpenGLWindow>());
                break;
            case RenderAPI::Vulkan:
                throw std::invalid_argument("Vulkan is not supported yet");
                break;
            }
            break;
        case WindowType::SDL:
            throw std::invalid_argument("SDL is not supported yet");
            break;
        default:
            throw std::invalid_argument("Invalid window type");
        }

        switch(render_api)
        {
        case RenderAPI::OpenGL:
            this->graphics_device_ =
                std::make_unique<GraphicsDevice>(std::make_shared<GLGraphicsDevice>(this->window_.get()));
            break;
        case RenderAPI::Vulkan:
            throw std::invalid_argument("Vulkan is not supported yet");
            break;
        default:
            throw std::invalid_argument("Invalid render API");
        }
    }
    Application::~Application()
    {
        this->graphics_device_.reset(nullptr);
        this->window_.reset(nullptr);
    }
    void Application::run() const
    {
        while(!window_->should_close())
        {
            window_->process_events();
            graphics_device_->present();
        }
    }
    Application& Application::get_instance(WindowType window_type, RenderAPI render_api)
    {
        static Application instance{ window_type, render_api };
        return instance;
    }
} // namespace zephyr