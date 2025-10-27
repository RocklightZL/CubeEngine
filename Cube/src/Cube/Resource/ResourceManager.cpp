#include "pch.h"
#include "ResourceManager.h"

#include "Cube/Renderer/Renderer.h"

namespace Cube {

    ResourceManager& ResourceManager::get() { return Renderer2D::currentContext->getResourceManager(); }

    Texture2D* ResourceManager::loadTexture(const std::string& path) {
        auto it = resourcesCache.find(path);
        if(it != resourcesCache.end()) {
            ResourceBase* resource = it->second.get();
            resource->refCount++;
            return static_cast<Texture2D*>(resource);
        }
        std::unique_ptr<Texture2D> res = std::make_unique<Texture2D>(path);
        Texture2D* ptr = res.get();
        res->refCount++;
        res->identifier = path;
        resourcesCache[path] = std::move(res);
        return ptr;
    }

    Font* ResourceManager::loadFont(const std::string& path, int fontSize) {
        std::string id = path + "&" + std::to_string(fontSize);
        auto it = resourcesCache.find(id);
        if(it != resourcesCache.end()) {
            ResourceBase* resource = it->second.get();
            resource->refCount++;
            return static_cast<Font*>(resource);
        }
        std::unique_ptr<Font> res = std::make_unique<Font>(path, fontSize);
        Font* ptr = res.get();
        res->refCount++;
        res->identifier = id;
        resourcesCache[id] = std::move(res);
        return ptr;
    }

    void ResourceManager::release(ResourceBase* resource) {
        if(!resource) return;
        resource->refCount--;
        if(resource->refCount == 0) {
            resourcesCache.erase(resource->identifier);
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