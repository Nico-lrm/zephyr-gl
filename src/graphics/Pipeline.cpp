#include "Pipeline.h"

#include <utility>

namespace zephyr {
    Pipeline::Pipeline(const GraphicPipelineCreateInfo& create_info)
    {
        // Hasher les infos des shaders
        // Si le shader existe, on mets la ref et c'est fini
        // Sinon, on créé l'objet
    }

    Pipeline::ShaderProgram::ShaderProgram() : ShaderProgram()
    {
        this->handle = glCreateProgram();
    }

    Pipeline::ShaderProgram::ShaderProgram(const GraphicShaderProgramInfos& create_info)
    {
        if(create_info.vertex_shader == nullptr || create_info.fragment_shader == nullptr)
            throw std::logic_error("ShaderProgram::ShaderProgram : Can't initialize graphic program without vertex and fragment shaders.");
        glAttachShader(this->handle, create_info.vertex_shader->get_handle());
        glAttachShader(this->handle, create_info.fragment_shader->get_handle());
        if(create_info.geometry_shader)
            glAttachShader(this->handle, create_info.geometry_shader->get_handle());
        if(create_info.tesselation_control_shader)
            glAttachShader(this->handle, create_info.tesselation_control_shader->get_handle());
        if(create_info.tesselation_evaluation_shader)
            glAttachShader(this->handle, create_info.tesselation_evaluation_shader->get_handle());
        glLinkProgram(this->handle);
        check_compile_errors(this->handle, "PROGRAM");
    }

    Pipeline::ShaderProgram::ShaderProgram(const ComputeShaderProgramInfos& create_info) : ShaderProgram()
    {
        if(create_info.compute_shader == nullptr)
            throw std::logic_error("ShaderProgram::ShaderProgram : Can't initialize compute program without compute shader.");
        glAttachShader(this->handle, create_info.compute_shader->get_handle());
        glLinkProgram(this->handle);
        check_compile_errors(this->handle, "PROGRAM");
    }

    Pipeline::ShaderProgram::~ShaderProgram() noexcept
    {
        if(handle != 0)
            glDeleteProgram(handle);
    }

    Pipeline::ShaderProgram::ShaderProgram(ShaderProgram&& old) noexcept
    {
        this->handle = std::exchange(old.handle, 0);
    }

    Pipeline::ShaderProgram& Pipeline::ShaderProgram::operator=(ShaderProgram&& old) noexcept
    {
        if(&old == this)
            return *this;
        if(handle != 0)
            glDeleteProgram(handle);
        this->handle = std::exchange(old.handle, 0);
        return *this;
    }

    GraphicsPipeline::GraphicsPipeline(const GraphicPipelineCreateInfo& create_info) : Pipeline(create_info)
    {
        // Hasher les infos des shaders
        // Si le VAO existe, on mets la ref et c'est fini
        // Sinon, on créé l'objet
    }

    GraphicsPipeline::VertexArray::VertexArray(std::span<VertexInputBinding> vertex_inputs)
    {
        glCreateVertexArrays(1, &handle);
        for(const auto& vertex_input : vertex_inputs) {
            glEnableVertexArrayAttrib(this->handle, vertex_input.location);
            switch (vertex_input.format) {
            case DataFormat::UNSIGNED_INT:
                glVertexArrayAttribIFormat(this->handle, vertex_input.location, vertex_input.count, GL_UNSIGNED_INT, static_cast<GLuint>(vertex_input.offset));
                break;
            case DataFormat::FLOAT:
                glVertexArrayAttribFormat(this->handle, vertex_input.location, vertex_input.count, GL_FLOAT, GL_FALSE, static_cast<GLuint>(vertex_input.offset));
                break;
            case DataFormat::DOUBLE:
                glVertexArrayAttribLFormat(this->handle, vertex_input.location, vertex_input.count, GL_DOUBLE, static_cast<GLuint>(vertex_input.offset));
                break;
            }
            glVertexArrayAttribBinding(this->handle, vertex_input.location, vertex_input.binding);
        }
    }

    GraphicsPipeline::VertexArray::~VertexArray() noexcept
    {
        if (handle != 0)
            glDeleteVertexArrays(1, &handle);
    }

    GraphicsPipeline::VertexArray::VertexArray(VertexArray&& old) noexcept
    {
        this->handle = std::exchange(old.handle, 0);
    }

    GraphicsPipeline::VertexArray& GraphicsPipeline::VertexArray::operator=(VertexArray&& old) noexcept
    {
        if (&old == this)
            return *this;
        if (handle != 0)
            glDeleteVertexArrays(1, &handle);
        this->handle = std::exchange(old.handle, 0);
        return *this;
    }
} // zephyr