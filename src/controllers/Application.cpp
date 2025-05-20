#include "Application.h"

namespace zephyr
{
    Application::Application()
    {

    }

    Application::~Application()
    {
        this->resource_manager_.reset();
        this->graphics_device_.reset();
        this->window_.reset();
    }

    void Application::run() const
    {
        while(!window_->should_close())
        {
            graphics_device_->present();
        }
    }

    Application& Application::get_instance()
    {
        static Application instance{};
        return instance;
    }
} // namespace zephyr