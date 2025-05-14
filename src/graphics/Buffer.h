#pragma once

#include "Utils.h"

#include <memory>
#include <span>
#include <vector>

namespace zephyr
{
    class BufferDataView : public std::span<const std::byte>
    {
      public:
        // For contiguous containers
        template<typename T>
            requires std::is_trivially_copyable_v<T>
        explicit BufferDataView(span<const T> data) : std::span<const std::byte>(std::as_bytes(data))
        {}

        // For a single value
        template<typename T>
            requires std::is_trivially_copyable_v<T>
        explicit BufferDataView(const T& data)
            : std::span<const std::byte>(std::as_bytes(std::span{ &data, static_cast<size_t>(1) }))
        {}
    };

    enum class BufferFlag
    {
        BUFFER_NONE = 0x00,
        BUFFER_DYNAMIC = 0x01,
        BUFFER_CLIENT = 0x02,
    };

    inline BufferFlag operator|(BufferFlag lhs, BufferFlag rhs)
    {
        using T = std::underlying_type_t<BufferFlag>;
        return static_cast<BufferFlag>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    inline BufferFlag operator&(BufferFlag lhs, BufferFlag rhs)
    {
        using T = std::underlying_type_t<BufferFlag>;
        return static_cast<BufferFlag>(static_cast<T>(lhs) & static_cast<T>(rhs));
    }

    inline BufferFlag& operator|=(BufferFlag& lhs, BufferFlag rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    class BufferFlags
    {
      public:
        using Underlying = std::underlying_type_t<BufferFlag>;

        constexpr BufferFlags() = default;
        constexpr explicit BufferFlags(BufferFlag flag) : bits(static_cast<Underlying>(flag)) {}

        constexpr BufferFlags& operator|=(BufferFlag flag)
        {
            bits |= static_cast<Underlying>(flag);
            return *this;
        }

        constexpr bool operator&(BufferFlag flag) const { return (bits & static_cast<Underlying>(flag)) != 0; }
        explicit constexpr operator bool() const { return bits != 0; }

      private:
        Underlying bits = 0;
    };

    constexpr BufferFlags DEFAULT_BUFFER_FLAGS{ BufferFlag::BUFFER_DYNAMIC | BufferFlag::BUFFER_CLIENT };

    struct BufferCreateInfo
    {
        BufferDataView buffer_data_view;
        BufferFlags flags{ DEFAULT_BUFFER_FLAGS };
    };

    struct ComposedBufferCreateInfo
    {
        std::vector<BufferDataView> buffers_data_view;
        BufferFlags flags{ DEFAULT_BUFFER_FLAGS };
    };

    struct AllocateBufferCreateInfo
    {
        long int size{};
        BufferFlags flags{ DEFAULT_BUFFER_FLAGS };
    };

    struct BufferUpdateInfo
    {
        BufferDataView buffer_data_view;
        size_t offset{ 0 };
    };

    class Buffer : public GLObject<unsigned int>
    {
        friend class GraphicsDevice;

      public:
        ~Buffer() noexcept;
        Buffer(Buffer&& old) noexcept;
        Buffer& operator=(Buffer&& old) noexcept;
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

      private:
        Buffer(const void* data, long int new_size, BufferFlags new_flags);
        long int size{ 0 };
        BufferFlags flags{};
    };

} // namespace zephyr