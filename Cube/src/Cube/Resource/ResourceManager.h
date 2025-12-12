#pragma once
#include "Resource.h"
#include "Cube/Core/Log.h"

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

        void registerAssetMeta(const AssetMeta& meta);
        void registerAssetMeta(const std::string& metaFilePath);

        // load
        template<typename T>
        T* load(RUID ruid) {
            static_assert(std::is_base_of_v<ResourceBase, T>);
            auto it = resourcesCache.find(ruid);
            if(it != resourcesCache.end()) {
                ResourceBase* resource = it->second.get();
                resource->refCount++;
                return static_cast<T*>(resource);
            }
            auto metaIt = assetMetaRegistry.find(ruid);
            if(metaIt == assetMetaRegistry.end()) {
                CB_CORE_ERROR("Failed to load resource: AssetMeta not found for RUID {}", ruid);
                return nullptr;
            }
            std::unique_ptr<T> res = std::make_unique<T>(metaIt->second);
            T* ptr = res.get();
            ++res->refCount;
            resourcesCache[ruid] = std::move(res);
            return ptr;
        }

        void release(ResourceBase* resource);
        void release(RUID ruid);
        void releaseAll();

    protected:
        ResourceManager() = default;

        virtual ~ResourceManager() = default;

        std::unordered_map<RUID, std::unique_ptr<ResourceBase>> resourcesCache;
        std::unordered_map<RUID, AssetMeta> assetMetaRegistry;
    };
}  // namespace Cube