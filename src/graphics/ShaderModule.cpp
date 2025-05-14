#include "ShaderModule.h"

#include <fstream>
#include <sstream>
#include <utility>

namespace zephyr
{
    ShaderModule::ShaderModule(const ShaderCreateInfo& create_info) : infos(create_info)
    {
        std::string shader_stage_string{};
        switch (create_info.shader_stage) {
        case ShaderStage::VERTEX:
            shader_stage_string = "VERTEX";
            break;
        case ShaderStage::FRAGMENT:
            shader_stage_string = "FRAGMENT";
            break;
        case ShaderStage::GEOMETRY:
            shader_stage_string = "GEOMETRY";
            break;
        case ShaderStage::TESSELATION_CONTROL:
            shader_stage_string = "TESSELATION_CONTROL";
            break;
        case ShaderStage::TESSELATION_EVALUATION:
            shader_stage_string = "TESSELATION_EVALUATION";
            break;
        case ShaderStage::COMPUTE:
            shader_stage_string = "COMPUTE";
            break;
        }
        std::string shader_code;
        std::ifstream shader_file;
        std::stringstream shader_stream;

        shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        shader_file.open(create_info.filepath.c_str());
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code = shader_stream.str();

        this->handle = glCreateShader(static_cast<GLenum>(this->infos.shader_stage));
        const char* code = shader_code.c_str();
        glShaderSource(handle, 1, &code, nullptr);
        glCompileShader(handle);
        check_compile_errors(handle, shader_stage_string);
    }
    ShaderModule::~ShaderModule() noexcept
    {
        if(handle != 0)
            glDeleteShader(handle);
    }
    ShaderModule::ShaderModule(ShaderModule&& old) noexcept : infos(std::exchange(old.infos, 0))
    {
        this->handle = std::exchange(old.handle, 0);
    }
    ShaderModule& ShaderModule::operator=(ShaderModule&& old) noexcept
    {
        if (&old == this)
            return *this;
        if(handle != 0)
            glDeleteShader(handle);
        this->handle = std::exchange(old.handle, 0);
        this->infos = std::exchange(old.infos, 0);
        return *this;
    }
    ShaderStage ShaderModule::get_shader_stage() const noexcept
    {
        return this->infos.shader_stage;
    }
    std::string_view ShaderModule::get_filepath() const noexcept
    {
        return this->infos.filepath;
    }
} // namespace zephyr