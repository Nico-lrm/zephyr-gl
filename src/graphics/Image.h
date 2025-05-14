#pragma once

#include "Types.h"
#include "Utils.h"

namespace zephyr
{
    struct ImageCreateInfo
    {
        ImageType type{ ImageType::TEXTURE_2D };
        ImageStorageFormat format{ ImageStorageFormat::RGBA8 };
        SampleCount sample_count{ SampleCount::SAMPLE_1 };
        Extent2D extent{};
        unsigned int mip_levels{ 1 };
        unsigned int layers{ 1 };
        bool generate_mipmap{ false };
    };

    struct ImageLoadInfo
    {
        std::string filepath;
        ImageType type{ ImageType::TEXTURE_2D };
        int num_mip_levels{ 1 };
        bool generate_mipmap{ false };
    };

    class Image : public GLObject<GLuint>
    {
        friend class GraphicsDevice;

      public:
        ~Image() noexcept;
        Image(const Image& old) = delete;
        Image& operator=(const Image& old) = delete;
        Image(Image&& old) noexcept;
        Image& operator=(Image&& old) noexcept;
        [[nodiscard]] ImageType get_texture_type() const noexcept;

      private:
        explicit Image(const ImageCreateInfo& create_info);
        explicit Image(const ImageLoadInfo& load_info);
        void load_compressed_texture(const ImageLoadInfo& load_info);
        void load_hdr_texture(const ImageLoadInfo& load_info);
        void load_classic_texture(const ImageLoadInfo& load_info);
        ImageCreateInfo texture_create_info{};
    };

    struct ImageViewCreateInfo
    {
        ImageStorageFormat internal_format{ ImageStorageFormat::SRGBA8 };
        unsigned int min_mip_level{ 0 };
        unsigned int num_mip_level{ 1 };
        unsigned int min_layer{ 0 };
        unsigned int num_layer{ 1 };
        ComponentSwizzle swizzles[4]{ ComponentSwizzle::RED, ComponentSwizzle::GREEN, ComponentSwizzle::BLUE,
                                      ComponentSwizzle::ALPHA };
    };

    class ImageView : public GLObject<GLuint>
    {
        friend class GraphicsDevice;

      public:
        ~ImageView() noexcept;
        ImageView(const ImageView& old) = delete;
        ImageView& operator=(const ImageView& old) = delete;
        ImageView(ImageView&& old) noexcept;
        ImageView& operator=(ImageView&& old) noexcept;

      private:
        ImageView();
        ImageView(GLuint texture_id, ImageType type, const ImageViewCreateInfo& create_info = {});
        explicit ImageView(const Image& texture, const ImageViewCreateInfo& create_info = {});
        void swizzle_components(const ImageViewCreateInfo& create_info) const;
    };

} // namespace zephyr
