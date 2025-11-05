#include "pch.h"
#include "ResourceManager.h"

#include "Cube/Renderer/Renderer.h"

namespace Cube {

    ResourceManager& ResourceManager::get() { return Renderer2D::currentContext->getResourceManager(); }

    void ResourceManager::release(ResourceBase* resource) {
        if(!resource) return;
        resource->refCount--;
        if(resource->refCount == 0) {
            resourcesCache.erase(resource->path);
        }
    }

    void ResourceManager::release(const std::string& identifier) {
        auto it = resourcesCache.find(identifier);
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