#include "pch.h"
#include "ResourceManager.h"

namespace Cube {

    ResourceManager& ResourceManager::getInstance() {
        static ResourceManager instance;
        return instance;
    }

    void ResourceManager::release(const std::string& path) {
        auto it = resourcesCache.find(path);
        if(it != resourcesCache.end()) {
            if((--it->second->refCount) == 0) {
                delete it->second;
                resourcesCache.erase(path);
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