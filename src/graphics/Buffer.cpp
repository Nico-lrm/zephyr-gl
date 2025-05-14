#include "Buffer.h"

#include <GL/glew.h>
#include <utility>

namespace zephyr
{
    Buffer::Buffer(const void* data, const long int new_size, const BufferFlags new_flags) : size(new_size), flags(new_flags)
    {
        glCreateBuffers(1, &handle);
        GLbitfield gl_flags{};
        if (new_flags & BufferFlag::BUFFER_DYNAMIC) {
            gl_flags |= GL_DYNAMIC_STORAGE_BIT;
        }
        if (new_flags & BufferFlag::BUFFER_CLIENT) {
            gl_flags |= GL_CLIENT_STORAGE_BIT;
        }
        glNamedBufferStorage(handle, new_size, data, gl_flags);
    }
    Buffer::~Buffer() noexcept
    {
        if (handle != 0)
            glDeleteBuffers(1, &handle);
    }
    Buffer::Buffer(Buffer&& old) noexcept : size(std::exchange(old.size, 0)), flags(std::exchange(old.flags, 0))
    {
        this->handle = std::exchange(old.handle, 0);
    }
    Buffer& Buffer::operator=(Buffer&& old) noexcept
    {
        if (&old == this)
            return *this;
        if(handle != 0)
            glDeleteBuffers(1, &handle);
        this->handle = std::exchange(old.handle, 0);
        this->size = std::exchange(old.size, 0);
        this->flags = std::exchange(old.flags, {});
        return *this;
    }
} // namespace zephyr