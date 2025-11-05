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
        template<typename T>
        T* load(const std::string& path) {
            static_assert(std::is_base_of_v<ResourceBase, T>);
            auto it = resourcesCache.find(path);
            if(it != resourcesCache.end()) {
                ResourceBase* resource = it->second.get();
                resource->refCount++;
                return static_cast<T*>(resource);
            }
            std::unique_ptr<T> res = std::make_unique<T>(path);
            T* ptr = res.get();
            ++res->refCount;
            res->path = path;
            resourcesCache[path] = std::move(res);
            return ptr;
        }

        void release(ResourceBase* resource);
        void release(const std::string& identifier);
        void releaseAll();

    protected:
        ResourceManager() = default;

        virtual ~ResourceManager() = default;

        std::unordered_map<std::string, std::unique_ptr<ResourceBase>> resourcesCache;
    };
}  // namespace Cube