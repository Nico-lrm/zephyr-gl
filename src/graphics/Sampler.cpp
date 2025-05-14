#include "Sampler.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

namespace zephyr {
    Sampler::Sampler(const SamplerCreateInfo& create_info)
    {
        glCreateSamplers(1, &handle);
        glSamplerParameteri(handle, GL_TEXTURE_COMPARE_MODE, create_info.compare_enable ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE);
        glSamplerParameteri(handle, GL_TEXTURE_COMPARE_FUNC, static_cast<GLint>(create_info.compare_operation));
        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(create_info.min_filter));
        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(create_info.mag_filter));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, static_cast<GLint>(create_info.address_mode));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, static_cast<GLint>(create_info.address_mode));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, static_cast<GLint>(create_info.address_mode));
        glSamplerParameterfv(handle, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(create_info.border_color));
        glSamplerParameterf(handle, GL_TEXTURE_MAX_ANISOTROPY, static_cast<GLfloat>(create_info.anisotropic_sample));
        glSamplerParameterf(handle, GL_TEXTURE_LOD_BIAS, create_info.lod_bias);
        glSamplerParameterf(handle, GL_TEXTURE_MIN_LOD, create_info.min_lod);
        glSamplerParameterf(handle, GL_TEXTURE_MAX_LOD, create_info.max_lod);
    }

    Sampler::~Sampler() noexcept
    {
        if (handle != 0)
            glDeleteSamplers(1, &handle);
    }

    Sampler::Sampler(Sampler&& old) noexcept
    {
        this->handle = std::exchange(old.handle, 0);
    }

    Sampler& Sampler::operator=(Sampler&& old) noexcept
    {
        if (&old == this)
            return *this;
        if (handle != 0)
            glDeleteSamplers(1, &handle);
        this->handle = std::exchange(old.handle, 0);
        return *this;
    }
} // zephyr