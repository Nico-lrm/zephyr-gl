#pragma once

#include "ShaderModule.h"
#include "Types.h"
#include "Utils.h"

#include <span>
#include <unordered_map>
#include <vector>

namespace zephyr
{
    struct VertexInputBinding
    {
        DataFormat format;     // glVertexArrayAttribFormat
        int count;             // glVertexArrayAttribFormat
        unsigned int location; // glEnableVertexArrayAttrib + glVertexArrayAttribFormat
        unsigned int binding;  // glVertexArrayAttribBinding
        size_t offset;         // glVertexArrayAttribFormat

        bool operator==(const VertexInputBinding& other) const
        {
            return format == other.format && count == other.count && location == other.location &&
                   binding == other.binding && offset == other.offset;
        }
    };

    struct GraphicShaderProgramInfos
    {
        ShaderModule* vertex_shader{ nullptr };
        ShaderModule* fragment_shader{ nullptr };
        ShaderModule* geometry_shader{ nullptr };
        ShaderModule* tesselation_control_shader{ nullptr };
        ShaderModule* tesselation_evaluation_shader{ nullptr };
    };

    struct InputAssemblyState
    {
        PrimitiveTopology topology{ PrimitiveTopology::TRIANGLES };
        bool primitive_restart_enable{ false };
        int primitive_restart_index{
            0
        }; // glPrimitiveRestartIndex(65535) - glEnable(GL_PRIMITIVE_RESTART|GL_PRIMITIVE_RESTART_INDEX)
    };

    struct RasterizationState
    {
        PolygonMode polygon_mode{ PolygonMode::FILL };        // glPolygonMode
        CullFace cull_face{ CullFace::NONE };                 // gl(En/Dis)able(GL_CULL_FACE) -> glCullFace
        FrontFace front_face{ FrontFace::COUNTER_CLOCKWISE }; // glFrontFace -> GL_CW / GL_CCW
        bool depth_clamping{ false };                         // gl(En/Dis)able(GL_DEPTH_CLAMP)
        bool rasterizer_discard{ false };                     // gl(En/Dis)able(GL_RASTERIZER_DISCARD)
        bool depth_bias_enable{ false };
        float depth_bias_const{ 0.f }; // glPolygonOffset
        float depth_bias_slope{ 0.f }; // glPolygonOffset
        float line_width{ 1.f };       // glLineWidth
        float point_size{ 1.f };       // glPointSize
    };

    struct MultisampleState
    {
        bool sample_shading{ false };           // glEnable(GL_SAMPLE_SHADING)
        float min_sample_shading{ 1.f };        // glMinSampleShading
        unsigned int sample_mask{ 0xFFFFFFFF }; // glSampleMask
        bool alpha_to_coverage{ false };        // glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE)
        bool alpha_to_one{ false };             // glEnable(GL_SAMPLE_ALPHA_TO_ONE)
    };

    struct DepthState
    {
        bool depth_test_enable{ true };                              // gl{Enable, Disable}(GL_DEPTH_TEST)
        bool depth_write_enable{ true };                             // glDepthMask(depthWriteEnable)
        CompareOperation depth_compare_op{ CompareOperation::LESS }; // glDepthFunc
    };

    struct ColorBlendAttachment
    {
        bool blend_enable{ false };
        BlendFactor src_color_blend_factor{ BlendFactor::ONE };  // glBlendFuncSeparate
        BlendFactor dst_color_blend_factor{ BlendFactor::ZERO }; // glBlendFuncSeparate
        BlendOperation color_blend_op = { BlendOperation::ADD }; // glBlendEquationSeparate
        BlendFactor src_alpha_blend_factor{ BlendFactor::ONE };  // glBlendFuncSeparate
        BlendFactor dst_alpha_blend_factor{ BlendFactor::ZERO }; // glBlendFuncSeparate
        BlendOperation alpha_blend_op{ BlendOperation::ADD };    // glBlendEquationSeparate
        ColorComponent color_write_mask{ ColorComponent::RGBA }; // glColorMask
    };

    struct ColorBlendState
    {
        bool logic_op_enable{ false };
        LogicOperator logic_operator{ LogicOperator::COPY };
        ColorBlendAttachment color_blend_attachment{};
        float blend_constant[4] = { 0, 0, 0, 0 };
    };

    struct GraphicPipelineCreateInfo
    {
        std::vector<VertexInputBinding> vertex_inputs_;
        GraphicShaderProgramInfos shader_program_infos_{};
        InputAssemblyState input_assembly_state_{};
        RasterizationState rasterization_state_{};
        MultisampleState multisample_state_{};
        DepthState depth_state_{};
        ColorBlendState color_blend_state_{};
    };

    struct ComputeShaderProgramInfos
    {
        ShaderModule* compute_shader{ nullptr };
    };

    class Pipeline
    {
      protected:
        explicit Pipeline(const GraphicPipelineCreateInfo& create_info);

        class ShaderProgram : public GLObject<GLuint>
        {
          public:
            explicit ShaderProgram(const GraphicShaderProgramInfos& create_info);
            explicit ShaderProgram(const ComputeShaderProgramInfos& create_info);
            ~ShaderProgram() noexcept;
            ShaderProgram(ShaderProgram&& old) noexcept;
            ShaderProgram& operator=(ShaderProgram&& old) noexcept;
            ShaderProgram(const ShaderProgram&) = delete;
            ShaderModule& operator=(const ShaderModule&) = delete;

          private:
            ShaderProgram();
        };
        ShaderProgram* shader_program_{ nullptr };
        static std::unordered_map<std::string, ShaderProgram> shader_programs_map_;
        static std::size_t hash_shader_program_infos(const GraphicShaderProgramInfos& infos)
        {
            std::size_t seed = 0;
            hash_combine(seed, hash_shader_module(infos.vertex_shader));
            hash_combine(seed, hash_shader_module(infos.fragment_shader));
            hash_combine(seed, hash_shader_module(infos.geometry_shader));
            hash_combine(seed, hash_shader_module(infos.tesselation_control_shader));
            hash_combine(seed, hash_shader_module(infos.tesselation_evaluation_shader));
            return seed;
        }
    };

    class GraphicsPipeline : public Pipeline
    {
        friend class GraphicsDevice;

        explicit GraphicsPipeline(const GraphicPipelineCreateInfo& create_info);
        GraphicPipelineCreateInfo graphic_pipeline_create_info;

        class VertexArray : public GLObject<unsigned int>
        {
          public:
            explicit VertexArray(std::span<VertexInputBinding> vertex_inputs);
            ~VertexArray() noexcept;
            VertexArray(VertexArray&& old) noexcept;
            VertexArray& operator=(VertexArray&& old) noexcept;
            VertexArray(const VertexArray&) = delete;
            VertexArray& operator=(const VertexArray&) = delete;
        };
        VertexArray* vertex_array_{ nullptr };
        static std::unordered_map<std::string, VertexArray> vertex_arrays_map_;
        static std::size_t hash_vertex_array_infos(const std::span<const VertexInputBinding> infos)
        {
            std::size_t seed = 0;
            for(const auto& info: infos)
            {
                hash_combine(seed, std::hash<int>{}(static_cast<int>(info.format)));
                hash_combine(seed, std::hash<int>{}(info.count));
                hash_combine(seed, std::hash<unsigned int>{}(info.location));
                hash_combine(seed, std::hash<unsigned int>{}(info.binding));
                hash_combine(seed, std::hash<size_t>{}(info.offset));
            }
            return seed;
        }
    };

} // namespace zephyr