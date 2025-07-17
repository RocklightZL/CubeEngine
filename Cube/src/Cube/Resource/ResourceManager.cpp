#include "pch.h"
#include "ResourceManager.h"

namespace Cube {

    ResourceManager& ResourceManager::getInstance() {
        static ResourceManager instance;
        return instance;
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