#include "domain/GraphicsDevice.h"
#include "domain/Window.h"
#include "providers/graphics/gl/GLGraphicsDevice.h"
#include "providers/windows/GLFWOpenGLWindow.h"

using namespace zephyr;

int main()
{
    std::unique_ptr<Window> window = std::make_unique<OpenGLWindow>(std::make_shared<GLFWOpenGLWindow>());
    auto graphics_device = std::make_unique<GraphicsDevice>(std::make_shared<GLGraphicsDevice>(window.get()));
    while(!window->should_close())
    {
        window->process_events();
        graphics_device->present();
    }
    graphics_device.reset(nullptr);
    window.reset(nullptr);

    return 0;
}
