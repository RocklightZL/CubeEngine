#pragma once
#include "Resource.h"
#include "Cube/Renderer/Shader.h"
#include "Cube/Renderer/Texture.h"

namespace Cube {

    class ResourceManager {
    public:
        static ResourceManager& getInstance();

        // Delete copy and move constructors and assignment operators
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

        template<typename T>
        Resource<T>* load(const std::string& path) {
            auto it = resourcesCache.find(path);
            if (it != resourcesCache.end()) {
                ResourceBase* resource = it->second;
                resource->refCount++;
                return static_cast<Resource<T>*>(resource);
            }

            Resource<T>* newResource = new Resource<T>();
            newResource->path = path;
            newResource->refCount = 1;

            if constexpr (std::is_base_of_v<T, Shader>) {
                newResource->data = new Shader(readFileToString(path));
            } else if constexpr (std::is_base_of_v<T, Texture2D>) {
                newResource->data = new Texture2D(path);
            } else {
                static_assert(false, "Unsupported resource type");
            }
            resourcesCache[path] = newResource;
            return newResource;
        }

        template<typename T>
        void release(Resource<T>*& resource) {
            if(resource && --(resource->refCount)) {
                resourcesCache.erase(resource->path);
                delete resource;
                resource = nullptr;
            }
        }

    private:
        ResourceManager() = default;
        ~ResourceManager() = default;

        std::map<std::string, ResourceBase*> resourcesCache;
    };

    template Resource<Texture2D>* ResourceManager::load(const std::string&);
    template Resource<Shader>* ResourceManager::load(const std::string&);

}  // namespace Cube