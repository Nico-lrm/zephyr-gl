#include "Utils.h"

#include <iostream>

namespace zephyr
{
    void opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        std::string sourceName;
        std::string typeName;
        std::string severityName;

        switch (source) {
            case 0x8248:
                sourceName = "GL_DEBUG_SOURCE_SHADER_COMPILER_ARB";
                break;
            case 0x8246:
                sourceName = "GL_DEBUG_SOURCE_API_ARB";
                break;
            default:
                sourceName = "GL_DEBUG_SOURCE_UNKNOWN";
                break;
        }

        switch (type) {
            case 0x824c:
                typeName = "GL_DEBUG_TYPE_ERROR_ARB";
                break;
            case 0x8250:
                typeName = "GL_DEBUG_TYPE_PERFORMANCE_ARB";
                break;
            default:
                typeName = "GL_DEBUG_TYPE_UNKNOWN";
                break;
        }

        switch(severity)
        {
        case 0x9146:
            severityName = "GL_DEBUG_SEVERITY_HIGH_AMD";
            break;
        case 0x9147:
            severityName = "GL_DEBUG_SEVERITY_MEDIUM_AMD";
            break;
        case 0x9148:
            severityName = "GL_DEBUG_SEVERITY_LOW_AMD";
            break;
        case 0x826b:
            severityName = "GL_DEBUG_SEVERITY_NOTIFICATION";
            break;
        default:
            sourceName = "GL_DEBUG_SEVERITY_UNKNOWN";
            break;
        }
        const std::string finalString = "source : " + sourceName + "\ntype : " + typeName + "\nseverity : " + severityName + "\nmessage : " + message;
        std::cout << finalString << "\n";
        //throw std::runtime_error(finalString.c_str());
    }

    void check_compile_errors(GLuint shader, const std::string& type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "Error compiling :" << type << "\n" << infoLog << "\n";
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "Error linking :" << type << "\n" << infoLog << "\n";
            }
        }
    }
}