#pragma once

#include "../domain/GraphicsDevice.h"
#include "../domain/Window.h"

#include <memory>

namespace zephyr
{
    enum class WindowType
    {
        GLFW,
        SDL,
    };

    enum class RenderAPI
    {
        OpenGL,
        Vulkan,
    };

    class Application
    {
      public:
        ~Application();
        Application(Application&& other) = delete;
        Application& operator=(Application&& other) = delete;
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void run() const;
        static Application& get_instance(WindowType window_type, RenderAPI render_api);

      private:
        Application(WindowType window_type, RenderAPI render_api);

        std::unique_ptr<Window> window_;
        std::unique_ptr<GraphicsDevice> graphics_device_;
    };
} // namespace zephyr
