#pragma once

#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <GL/glew.h>

namespace zephyr
{
    static constexpr std::string_view APP_NAME = "ZephyrEngine";

    struct Offset2D
    {
        int x_{ 0 };
        int y_{ 0 };
    };

    struct Extent2D
    {
        int width_{ 0 };
        int height_{ 0 };
    };

    struct Viewport
    {
        Offset2D offset_{};
        Extent2D extent_{};
        float near_{ 0.01f };
        float far_{ 3000.f };
    };

    enum class FrameState
    {
        RECORDING,
        END_RECORD,
        SUBMITTED,
        READY
    };

    enum class DataFormat : GLint {
        UNSIGNED_INT = GL_UNSIGNED_INT,
        FLOAT = GL_FLOAT,
        DOUBLE = GL_DOUBLE
    };

    enum class PolygonMode : GLenum {
        POINT = GL_POINT,
        LINE = GL_LINE,
        FILL = GL_FILL
    };

    enum class CullFace : GLenum {
        FRONT = GL_FRONT,
        BACK = GL_BACK,
        FRONT_BACK = GL_FRONT_AND_BACK,
        NONE = 0
    };
    enum class FrontFace : GLenum {
        CLOCKWISE = GL_CW,
        COUNTER_CLOCKWISE = GL_CCW
    };

    enum class PrimitiveTopology : GLenum {
        POINTS = GL_POINTS,
        LINES = GL_LINES,
        LINE_LOOP = GL_LINE_LOOP,
        LINE_STRIP = GL_LINE_STRIP,
        LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY, // For GeometryShader
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_ADJACENCY = GL_TRIANGLES_ADJACENCY, // For GeometryShader
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY, // For GeometryShader
        TRIANGLE_FAN = GL_TRIANGLE_FAN
    };

    enum class SampleCount : GLuint {
        SAMPLE_1 = 1,
        SAMPLE_2 = 2,
        SAMPLE_4 = 4,
        SAMPLE_8 = 8,
        SAMPLE_16 = 16
    };

    enum class CompareOperation : GLenum {
        NEVER = GL_NEVER,
        LESS = GL_LESS,
        GREATER = GL_GREATER,
        EQUAL = GL_EQUAL,
        ALWAYS = GL_ALWAYS,
        LEQUAL = GL_LEQUAL,
        GEQUAL = GL_GEQUAL,
        NOT_EQUAL = GL_NOTEQUAL
    };

    enum class BlendOperation : GLenum {
        ADD = GL_FUNC_ADD,
        SUBTRACT = GL_FUNC_SUBTRACT,
        REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
        MIN = GL_MIN,
        MAX = GL_MAX
    };

    enum class LogicOperator : GLenum {
        CLEAR = GL_CLEAR,
        SET = GL_SET,
        COPY = GL_COPY,
        COPY_INVERTED = GL_COPY_INVERTED,
        NOOP = GL_NOOP,
        INVERT = GL_INVERT,
        AND = GL_AND,
        NAND = GL_NAND,
        OR = GL_OR,
        NOR = GL_NOR,
        XOR = GL_XOR,
        EQUIV = GL_EQUIV,
        AND_REVERSE = GL_AND_REVERSE,
        AND_INVERTED = GL_AND_INVERTED,
        OR_REVERSE = GL_OR_REVERSE,
        OR_INVERTED = GL_AND_INVERTED
    };

    enum class BlendFactor : GLenum {
        ZERO = GL_ZERO,
        ONE = GL_ONE,
        SRC_COLOR = GL_SRC_COLOR,
        ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
        DST_COLOR = GL_DST_COLOR,
        ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
        SRC_ALPHA = GL_SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        DST_ALPHA = GL_DST_ALPHA,
        ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR = GL_CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
        SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
        SRC1_COLOR = GL_SRC1_COLOR,
        ONE_MINUS_SRC1_COLOR = GL_ONE_MINUS_SRC1_COLOR,
        SRC1_ALPHA = GL_SRC1_ALPHA,
        ONE_MINUS_SRC1_ALPHA = GL_ONE_MINUS_SRC1_ALPHA,
    };

    enum class ImageType : GLenum {
        TEXTURE_2D = GL_TEXTURE_2D,
        TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
        TEXTURE_CUBEMAP = GL_TEXTURE_CUBE_MAP
    };

    enum class ImageStorageFormat : GLint {
        DEPTH16 = GL_DEPTH_COMPONENT16,
        DEPTH24 = GL_DEPTH_COMPONENT24,
        DEPTH32F = GL_DEPTH_COMPONENT32F,
        STENCIL8 = GL_STENCIL_INDEX8,
        DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
        DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
        SRGB8 = GL_SRGB8,
        RGB8 = GL_RGB8,
        SRGBA8 = GL_SRGB8_ALPHA8,
        RGBA8 = GL_RGBA8,
        RGB16 = GL_RGB16,
        RGB10A2 = GL_RGB10_A2,
        RGBA12 = GL_RGBA12,
        RGB12 = GL_RGB12,
        RGB16F = GL_RGB16F,
        RGBA16 = GL_RGBA16,
        RGBA16F = GL_RGBA16F,
        RGB32F = GL_RGB32F,
        RGBA32F = GL_RGBA32F,
        BPTC_UNORM_RGBA = GL_COMPRESSED_RGBA_BPTC_UNORM,
        BPTC_UNORM_SRGBA = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,

    };

    enum class Filter : GLint {
        LINEAR = GL_LINEAR,
        NEAREST = GL_NEAREST,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class AddressMode : GLint {
        REPEAT = GL_REPEAT,
        CLAMP_BORDER = GL_CLAMP_TO_BORDER,
        CLAMP_EDGE = GL_CLAMP_TO_EDGE
    };

    enum class ComponentSwizzle : GLint {
        RED = GL_RED,
        BLUE = GL_BLUE,
        GREEN = GL_GREEN,
        ALPHA = GL_ALPHA,
        ONE = GL_ONE,
        ZERO = GL_ZERO
    };

    enum class AttachmentLoadOperation : GLbitfield {
        DONT_CARE = GL_DONT_CARE,
        LOAD = GL_LOAD,
        CLEAR = GL_CLEAR
    };

    enum class BufferBit : GLbitfield {
        COLOR = GL_COLOR_BUFFER_BIT,
        DEPTH = GL_DEPTH_BUFFER_BIT,
        STENCIL = GL_STENCIL_BUFFER_BIT
    };

    enum class ShaderStage : GLenum {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER,
        TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER,
        TESSELATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
        COMPUTE = GL_COMPUTE_SHADER
    };

    enum class ColorComponent {
        RGBA,
        RBA,
        RGA,
        RB,
        RG,
        R,
        BGA,
        BA,
        B,
        GA,
        G
    };

}; // namespace zephyr
