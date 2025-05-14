#include "Image.h"

#include <ktx.h>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace zephyr
{
    Image::Image(const ImageCreateInfo& create_info) : texture_create_info(create_info)
    {
        glCreateTextures(static_cast<GLenum>(create_info.type), 1, &handle);
        switch(create_info.type)
        {
        case ImageType::TEXTURE_2D:
        case ImageType::TEXTURE_CUBEMAP:
            glTextureStorage2D(this->handle, static_cast<GLsizei>(create_info.mip_levels),
                               static_cast<GLenum>(create_info.format), static_cast<GLsizei>(create_info.extent.width_),
                               static_cast<GLsizei>(create_info.extent.height_));
            break;
        case ImageType::TEXTURE_2D_MULTISAMPLE:
            glTextureStorage2DMultisample(this->handle, static_cast<GLsizei>(create_info.sample_count),
                                          static_cast<GLenum>(create_info.format),
                                          static_cast<GLsizei>(create_info.extent.width_),
                                          static_cast<GLsizei>(create_info.extent.height_), GL_TRUE);
            break;
        }
    }

    Image::~Image() noexcept
    {
        if(handle)
            glDeleteTextures(1, &handle);
    }

    Image::Image(Image&& old) noexcept
    {
        this->handle = std::exchange(old.handle, 0);
        this->texture_create_info = std::exchange(old.texture_create_info, {});
    }

    Image& Image::operator=(Image&& old) noexcept
    {
        if(&old == this)
            return *this;
        if(handle)
            glDeleteTextures(1, &handle);
        this->handle = std::exchange(old.handle, 0);
        this->texture_create_info = std::exchange(old.texture_create_info, {});
        return *this;
    }

    ImageType Image::get_texture_type() const noexcept
    {
        return this->texture_create_info.type;
    }

    Image::Image(const ImageLoadInfo& load_info)
    {
        if(load_info.type == ImageType::TEXTURE_2D_MULTISAMPLE)
        {
            throw std::invalid_argument("Texture2D Multisample can't be created from file.");
        }

        const auto extension{ load_info.filepath.substr(load_info.filepath.find_last_of('.') + 1) };
        if(extension == "ktx2")
            load_compressed_texture(load_info);
        else if(extension == "hdr")
            load_hdr_texture(load_info);
        else
            throw std::invalid_argument("Only .ktx2 and .hdr files are allowed.");
    }

    void Image::load_compressed_texture(const ImageLoadInfo& load_info)
    {
        // Decompress texture with libktx
        ktxTexture2* ktx_texture;
        ktxTexture2_CreateFromNamedFile(load_info.filepath.c_str(), KTX_TEXTURE_CREATE_NO_FLAGS, &ktx_texture);
        if(!ktx_texture->isCubemap && load_info.type == ImageType::TEXTURE_CUBEMAP)
        {
            ktxTexture_Destroy(ktxTexture(ktx_texture));
            throw std::invalid_argument("File is not a cubemap texture while asked texture type is cubemap.\n");
        }
        GLenum texture_format{};
        if(ktxTexture2_NeedsTranscoding(ktx_texture))
        {
            ktx_texture_transcode_fmt_e tf{};
            khr_df_model_e colorModel = ktxTexture2_GetColorModel_e(ktx_texture);
            if(colorModel == KHR_DF_MODEL_UASTC)
            {
                if(GLEW_KHR_texture_compression_astc_ldr)
                {
                    texture_format = GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
                    tf = KTX_TTF_ASTC_4x4_RGBA;
                }
                else
                {
                    texture_format = GL_COMPRESSED_RGBA_BPTC_UNORM;
                    tf = KTX_TTF_BC7_RGBA;
                }
            }
            else if(colorModel == KHR_DF_MODEL_ETC1S)
            {
                texture_format = GL_COMPRESSED_RGB8_ETC2;
                tf = KTX_TTF_ETC2_RGBA;
            }
            else if(colorModel == KHR_DF_MODEL_ETC2)
            {
                texture_format = GL_COMPRESSED_RGBA8_ETC2_EAC;
                tf = KTX_TTF_ETC2_RGBA;
            }
            else
            {
                throw std::runtime_error("Compression method should be UASTC, ETC1S or ETC2");
            }
            ktxTexture2_TranscodeBasis(ktx_texture, tf, 0);
        }

        // Find number of mips available
        GLint num_mip{ 1 };
        if(load_info.generate_mipmap && load_info.num_mip_levels > 1)
        {
            bool finished = false;
            while(!finished)
            {
                const auto width = std::max(1, static_cast<GLint>(ktx_texture->baseWidth) >> num_mip);
                const auto height = std::max(1, static_cast<GLint>(ktx_texture->baseHeight) >> num_mip);
                if(width == 1 || height == 1 || num_mip == load_info.num_mip_levels - 1)
                {
                    finished = true;
                }
                num_mip++;
            }
        }

        // Update creation infos
        this->texture_create_info.type = load_info.type;
        this->texture_create_info.extent = { static_cast<GLint>(ktx_texture->baseWidth),
                                             static_cast<GLint>(ktx_texture->baseHeight) };
        this->texture_create_info.mip_levels = num_mip;
        this->texture_create_info.layers = ktx_texture->numFaces;

        // Create OpenGL texture using data gathered
        glCreateTextures(static_cast<GLenum>(texture_create_info.type), 1, &handle);
        glTextureStorage2D(this->handle, this->texture_create_info.mip_levels, texture_format,
                           texture_create_info.extent.width_, texture_create_info.extent.height_);
        if(texture_create_info.type == ImageType::TEXTURE_2D)
        {
            glCompressedTextureSubImage2D(this->handle, 0, 0, 0, this->texture_create_info.extent.width_,
                                          this->texture_create_info.extent.height_, texture_format,
                                          static_cast<GLsizei>(ktx_texture->dataSize), ktx_texture->pData);
        }
        else if(texture_create_info.type == ImageType::TEXTURE_CUBEMAP)
        {
            auto individual_size{ ktx_texture->dataSize / ktx_texture->numFaces };
            for(int face = 0; face < static_cast<int>(ktx_texture->numFaces); face++)
            {
                glCompressedTextureSubImage3D(this->handle, 0, 0, 0, face, this->texture_create_info.extent.width_,
                                              this->texture_create_info.extent.height_, static_cast<GLsizei>(1.f),
                                              texture_format, static_cast<GLsizei>(individual_size),
                                              ktx_texture->pData + (individual_size * face));
            }
        }
        if(load_info.generate_mipmap && num_mip > 1)
            glGenerateTextureMipmap(handle);
        ktxTexture_Destroy(ktxTexture(ktx_texture));
    }

    void Image::load_hdr_texture(const ImageLoadInfo& load_info)
    {
        stbi_set_flip_vertically_on_load(true);
        int nr_component{};
        auto* data{ stbi_loadf(load_info.filepath.c_str(), &texture_create_info.extent.width_,
                               &texture_create_info.extent.height_, &nr_component, 0) };
        if(data)
        {
            glCreateTextures(static_cast<GLenum>(texture_create_info.type), 1, &handle);
            glTextureStorage2D(this->handle, 1, GL_RGB16F, texture_create_info.extent.width_,
                               texture_create_info.extent.height_);
            glTextureSubImage2D(this->handle, 0, 0, 0, texture_create_info.extent.width_,
                                texture_create_info.extent.height_, GL_RGB, GL_FLOAT, data);
            this->texture_create_info.type = load_info.type;
            this->texture_create_info.mip_levels = 1;
            this->texture_create_info.layers = 1;
            stbi_image_free(data);
        }
        else
        {
            throw std::runtime_error("Failed to load HDR image.");
        }
    }

    void Image::load_classic_texture(const ImageLoadInfo& load_info)
    {
        // TODO : Implement this
    }

    ImageView::ImageView()
    {
        glGenTextures(1, &handle);
    }

    ImageView::ImageView(const Image& texture, const ImageViewCreateInfo& create_info)
        : ImageView(texture.get_handle(), texture.get_texture_type(), create_info)
    {}

    ImageView::ImageView(GLuint texture_id, ImageType type, const ImageViewCreateInfo& create_info) : ImageView()
    {
        glTextureView(handle, static_cast<GLenum>(type), texture_id, static_cast<GLenum>(create_info.internal_format),
                      create_info.min_mip_level, create_info.num_mip_level, create_info.min_layer,
                      create_info.num_layer);
        swizzle_components(create_info);
    }

    ImageView::~ImageView() noexcept
    {
        if(handle)
            glDeleteTextures(1, &handle);
    }

    ImageView::ImageView(ImageView&& old) noexcept
    {
        this->handle = std::exchange(old.handle, 0);
    }

    ImageView& ImageView::operator=(ImageView&& old) noexcept
    {
        if(&old == this)
            return *this;
        if(handle)
            glDeleteTextures(1, &handle);
        this->handle = std::exchange(old.handle, 0);
        return *this;
    }

    void ImageView::swizzle_components(const ImageViewCreateInfo& create_info) const
    {
        glTextureParameteri(handle, GL_TEXTURE_SWIZZLE_R, static_cast<GLint>(create_info.swizzles[0]));
        glTextureParameteri(handle, GL_TEXTURE_SWIZZLE_G, static_cast<GLint>(create_info.swizzles[1]));
        glTextureParameteri(handle, GL_TEXTURE_SWIZZLE_B, static_cast<GLint>(create_info.swizzles[2]));
        glTextureParameteri(handle, GL_TEXTURE_SWIZZLE_A, static_cast<GLint>(create_info.swizzles[3]));
    }
} // namespace zephyr