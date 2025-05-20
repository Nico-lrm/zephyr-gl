#pragma once

#include "GraphicsDevice.h"

#include <functional>
#include <unordered_map>

namespace zephyr
{
    struct RenderTarget
    {
        bool resize_on_window_resize = true;
    };

    struct RenderPass {
        std::string name;
        std::string render_target_name;
        bool clear_color = true;
        bool clear_depth = true;
        std::function<void(GraphicsDevice*)> render_callback;
    };

    class Renderer
    {
      public:
        explicit Renderer(std::shared_ptr<GraphicsDevice> graphics_device);

        void render();
        void add_render_pass(const RenderPass& render_pass);
        void add_render_target(const RenderTarget& render_target);

      private:
        std::shared_ptr<GraphicsDevice> graphics_device_;

        std::unordered_map<std::string, RenderPass> render_passes_;
        std::unordered_map<std::string, RenderTarget> render_targets_;
    };

} // namespace zephyr
