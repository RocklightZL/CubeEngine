#include "pch.h"
#include "ResourceManager.h"

#include "Cube/Renderer/Renderer.h"

namespace Cube {

    ResourceManager& ResourceManager::getInstance() {
        return Renderer2D::currentContext->getResourceManager();
    }

    void ResourceManager::release(const std::string& path) {
        auto it = resourcesCache.find(path);
        if(it != resourcesCache.end()) {
            if((--it->second->refCount) == 0) {
                delete it->second;
                resourcesCache.erase(it);
            }
        }
    }

    void ResourceManager::releaseAll() {
        for(auto& resource : resourcesCache) {
            delete resource.second;
        }
        resourcesCache.clear();
    }

    ResourceManager::~ResourceManager() {
        for(auto& resource : resourcesCache) {
            delete resource.second;
        }
    }
}  // namespace Cube