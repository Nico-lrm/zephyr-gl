#pragma once

#include "Types.h"
#include "Utils.h"

#include <glm/glm.hpp>

namespace zephyr
{

    struct SamplerCreateInfo
    {
        Filter min_filter{ Filter::LINEAR };
        Filter mag_filter{ Filter::LINEAR };
        AddressMode address_mode{ AddressMode::REPEAT };
        SampleCount anisotropic_sample{ SampleCount::SAMPLE_4 };
        glm::vec4 border_color{ 1.f };
        float lod_bias{ 0.f };
        float min_lod{ -1000.f };
        float max_lod{ 1000.f };
        bool compare_enable{ false };
        CompareOperation compare_operation{ CompareOperation::NEVER };
    };

    class Sampler : public GLObject<unsigned int>
    {
        friend class GraphicsDevice;

      public:
        ~Sampler() noexcept;
        Sampler(const Sampler& old) = delete;
        Sampler& operator=(const Sampler& old) = delete;
        Sampler(Sampler&& old) noexcept;
        Sampler& operator=(Sampler&& old) noexcept;

      private:
        explicit Sampler(const SamplerCreateInfo& create_info);
    };

} // namespace zephyr
