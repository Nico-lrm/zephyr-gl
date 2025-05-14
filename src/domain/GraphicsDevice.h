#pragma once

#include "../graphics/Buffer.h"
#include "../graphics/Image.h"
#include "../graphics/Pipeline.h"
#include "../graphics/Sampler.h"
#include "../graphics/ShaderModule.h"
#include "../graphics/Types.h"

#include <unordered_map>

namespace zephyr
{
    struct StateTracker
    {
        FrameState current_frame_state{ FrameState::READY };
        Viewport viewport{};
        Pipeline* current_pipeline{ nullptr };
    };

    class GraphicsDevice
    {
      public:
        explicit GraphicsDevice();
        ~GraphicsDevice() = default;
        GraphicsDevice(GraphicsDevice&& other) noexcept;
        GraphicsDevice& operator=(GraphicsDevice&& other) noexcept;
        GraphicsDevice(const GraphicsDevice&) = delete;
        GraphicsDevice& operator=(const GraphicsDevice&) = delete;

        void update_buffer(std::shared_ptr<Buffer> buffer, const BufferUpdateInfo& update_info);
        std::shared_ptr<Buffer> allocate_buffer(const BufferCreateInfo& create_info);
        std::shared_ptr<Buffer> allocate_buffer(const ComposedBufferCreateInfo& create_info);
        std::shared_ptr<Buffer> allocate_buffer(const AllocateBufferCreateInfo& allocate_buffer_create_info);
        void allocate_sampler(std::string name, const SamplerCreateInfo& create_info);
        void allocate_shader_module(std::string name, const ShaderCreateInfo& create_info);
        void allocate_graphic_pipeline(std::string name, const GraphicPipelineCreateInfo& create_info);
        void allocate_image(std::string name, const ImageCreateInfo& create_info);
        void allocate_image(std::string name, const ImageLoadInfo& create_info);
        std::shared_ptr<Sampler> get_sampler(std::string name);
        std::shared_ptr<ShaderModule> get_shader_module(std::string name);
        std::shared_ptr<Pipeline> get_graphic_pipeline(std::string name);
        std::shared_ptr<ImageView> get_image(std::string name, const ImageViewCreateInfo& create_info);

      private:
        StateTracker tracker{};
        std::unordered_map<std::string, std::shared_ptr<Image>> images_;
        std::unordered_map<std::string, std::shared_ptr<Sampler>> samplers_;
        std::unordered_map<std::string, std::shared_ptr<ShaderModule>> shader_modules_;
        std::unordered_map<std::string, std::shared_ptr<Pipeline>> pipelines_;
    };
} // namespace zephyr
