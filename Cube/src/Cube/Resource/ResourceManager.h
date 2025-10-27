#pragma once
#include "Resource.h"
#include "Cube/Renderer/Font.h"
#include "Cube/Renderer/Shader.h"
#include "Cube/Renderer/Texture.h"
#include "Cube/Utils/Utils.h"

namespace Cube {
    class Context;

    class ResourceManager {
    public:
        friend Context;

        // not a singleton pattern
        static ResourceManager& get();

        // Delete copy and move constructors and assignment operators
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

        // load
        Texture2D* loadTexture(const std::string& path);
        Font* loadFont(const std::string& path, int fontSize);

        void release(ResourceBase* resource);
        void release(const std::string& identifier);
        void releaseAll();

    protected:
        ResourceManager() = default;

        virtual ~ResourceManager() = default;

        std::unordered_map<std::string, std::unique_ptr<ResourceBase>> resourcesCache;
    };
}  // namespace Cube