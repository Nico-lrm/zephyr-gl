#pragma once

#include <GL/glew.h>
#include <string>

namespace zephyr
{
    void opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
    void check_compile_errors(GLuint shader, const std::string& type);

    template<typename T>
    void hash_combine(std::size_t& seed, const T& value)
    {
        std::hash<T> hasher;
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<typename T>
    class GLObject
    {
      public:
        [[nodiscard]] T get_handle() const noexcept { return this->handle; }

      protected:
        GLObject() = default;
        T handle{};
    };
} // namespace zephyr