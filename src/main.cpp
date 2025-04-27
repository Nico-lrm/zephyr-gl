#include "controllers/Application.h"

#include <iostream>

using namespace zephyr;

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <window type> <render API>" << std::endl;
        return 1;
    }

    WindowType window_type;
    RenderAPI render_api;

    if(std::string_view{ argv[1] } == "glfw")
        window_type = WindowType::GLFW;
    else if(std::string_view{ argv[1] } == "sdl")
        window_type = WindowType::SDL;
    else
        throw std::invalid_argument("Invalid window type");

    if(std::string_view{ argv[2] } == "opengl")
        render_api = RenderAPI::OpenGL;
    else if(std::string_view{ argv[2] } == "vulkan")
        render_api = RenderAPI::Vulkan;
    else
        throw std::invalid_argument("Invalid render API");

    try
    {
        const auto& app = Application::get_instance(window_type, render_api);
        app.run();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
