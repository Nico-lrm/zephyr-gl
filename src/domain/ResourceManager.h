#pragma once

#include "../resources/Mesh.h"
#include "../resources/Material.h"
#include "../resources/Texture.h"
#include "GraphicsDevice.h"

namespace zephyr
{
    class ResourceManager
    {
      public:
        explicit ResourceManager(std::shared_ptr<GraphicsDevice> graphics_device);

      private:
        std::shared_ptr<GraphicsDevice> graphics_device_;

        std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes_;
        std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures_;
    };
} // namespace zephyr
