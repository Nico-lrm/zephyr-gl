#pragma once

#include "../domain/GraphicsDevice.h"
#include "../domain/Renderer.h"
#include "../domain/ResourceManager.h"
#include "../domain/Window.h"

#include <memory>

namespace zephyr
{
    class Application
    {
      public:
        ~Application();
        Application(Application&& other) = delete;
        Application& operator=(Application&& other) = delete;
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void run() const;
        static Application& get_instance();

      private:
        Application();

        std::shared_ptr<Window> window_;
        std::shared_ptr<GraphicsDevice> graphics_device_;
        std::shared_ptr<ResourceManager> resource_manager_;
        std::shared_ptr<Renderer> renderer_;
    };
} // namespace zephyr
