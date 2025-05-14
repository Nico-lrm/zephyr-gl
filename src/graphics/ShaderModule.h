#pragma once

#include "Types.h"
#include "Utils.h"

#include <string>

namespace zephyr
{
    struct ShaderCreateInfo
    {
        std::string filepath;
        ShaderStage shader_stage;
    };

    class ShaderModule : public GLObject<GLuint>
    {
        friend class GraphicsDevice;

      public:
        ~ShaderModule() noexcept;
        ShaderModule(ShaderModule&& old) noexcept;
        ShaderModule& operator=(ShaderModule&& old) noexcept;
        ShaderModule(const ShaderModule&) = delete;
        ShaderModule& operator=(const ShaderModule&) = delete;
        [[nodiscard]] ShaderStage get_shader_stage() const noexcept;
        [[nodiscard]] std::string_view get_filepath() const noexcept;

      private:
        explicit ShaderModule(const ShaderCreateInfo& create_info);
        ShaderCreateInfo infos;
    };

    inline std::size_t hash_shader_module(const ShaderModule* module)
    {
        if (!module) return 0;
        std::size_t seed = 0;
        hash_combine(seed, std::hash<std::string_view>()(module->get_filepath()));
        hash_combine(seed, static_cast<std::underlying_type_t<ShaderStage>>(module->get_shader_stage()));
        return seed;
    }

} // namespace zephyr
