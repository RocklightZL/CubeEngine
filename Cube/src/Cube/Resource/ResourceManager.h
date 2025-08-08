#pragma once
#include "Resource.h"
#include "Cube/Renderer/Shader.h"
#include "Cube/Renderer/Texture.h"
#include "Cube/Utils/Utils.h"

namespace Cube {
    class Context;

    class ResourceManager {
    public:
        friend Context;

        // 并非单例模式
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

            if constexpr (std::is_same_v<T, Shader>) {
                newResource->data = new Shader(Utils::readFileToString(path));
            } else if constexpr (std::is_same_v<T, Texture2D>) {
                newResource->data = new Texture2D(path);
            } else {
                static_assert(false, "Unsupported resource type");
            }
            resourcesCache[path] = newResource;
            return newResource;
        }

        // with metaData
        template<typename T>
        Resource<T>* load(const std::string& filePath, const std::string& metaDataPath) {
            auto it = resourcesCache.find(filePath);
            if(it != resourcesCache.end()) {
                it->second->refCount++;
                return static_cast<Resource<T>*>(it->second);
            }

            Resource<T>* newResource = new Resource<T>();
            newResource->path = filePath;
            newResource->refCount = 1;
            if constexpr (std::is_same_v<T, TextureAtlas>) {
                newResource->data = new TextureAtlas(filePath, metaDataPath);
            } else {
                static_assert(false, "Unsupported resource type with metadata");
            }
            resourcesCache[filePath] = newResource;
            return newResource;
        }

        template<typename T>
        void release(Resource<T>*& resource) {
            if(resource && (--resource->refCount) == 0) {
                resourcesCache.erase(resource->path);
                delete resource;
                resource = nullptr;
            }
        }

        void release(const std::string& path);

        void releaseAll();

    protected:
        ResourceManager() = default;

        virtual ~ResourceManager();

        std::unordered_map<std::string, ResourceBase*> resourcesCache;
    };

    template Resource<Texture2D>* ResourceManager::load(const std::string&);
    template Resource<Shader>* ResourceManager::load(const std::string&);

}  // namespace Cube