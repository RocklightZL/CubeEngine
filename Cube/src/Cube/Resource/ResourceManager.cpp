#include "pch.h"
#include "ResourceManager.h"

#include <fstream>

#include "Cube/Renderer/Renderer.h"

namespace Cube {

    ResourceManager& ResourceManager::get() {
        return Renderer2D::currentContext->getResourceManager();
    }

    AssetMeta* ResourceManager::registerAssetMeta(const AssetMeta& meta) {
        assetMetaRegistry[meta.ruid] = meta;
        return &assetMetaRegistry[meta.ruid];
    }

    AssetMeta* ResourceManager::registerAssetMeta(const std::string& metaFilePath) {
        std::ifstream metaFile(metaFilePath);
        if(!metaFile.is_open()) {
            CB_CORE_ERROR("Failed to open meta file: {}", metaFilePath);
            return nullptr;
        }
        nlohmann::json j;
        metaFile >> j;
        metaFile.close();
        AssetMeta meta;
        meta.fromJson(j);
        return registerAssetMeta(meta);
    }

    AssetMeta* ResourceManager::getAssetMeta(RUID ruid) {
        auto it = assetMetaRegistry.find(ruid);
        if(it != assetMetaRegistry.end()) {
            return &it->second;
        }
        return nullptr;
    }

    void ResourceManager::release(ResourceBase* resource) {
        if(!resource) return;
        resource->refCount--;
        if(resource->refCount == 0) {
            resourcesCache.erase(resource->ruid);
        }
    }

    void ResourceManager::release(RUID ruid) {
        auto it = resourcesCache.find(ruid);
        if(it != resourcesCache.end()) {
            if((--it->second->refCount) == 0) {
                resourcesCache.erase(it);
            }
        }
    }

    void ResourceManager::releaseAll() {
        resourcesCache.clear();
    }
}  // namespace Cube