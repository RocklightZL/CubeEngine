#include "pch.h"

#include "ResourceManager.h"

#include "ResPtr.h"
#include "Sprite.h"
#include "Cube/Animation/AnimationClip.h"
#include "Cube/Renderer/Font.h"

#include <fstream>

#include "Cube/Renderer/Renderer.h"

namespace Cube {

    ResourceManager& ResourceManager::get() {
        static thread_local ResourceManager instance;
        return instance;
    }

    void ResourceManager::init(const std::string& pathMapFilePath) {
        std::ifstream file(pathMapFilePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("Failed to open resource path map file: {}", pathMapFilePath);
        }
        nlohmann::json jsonData;
        file >> jsonData;
        file.close();
        get().pathMap = jsonData.get<std::unordered_map<std::string, nlohmann::json>>();
    }

    void ResourceManager::init(const std::unordered_map<std::string, nlohmann::json>& pathMap) {
        get().pathMap = pathMap;                        
    }

    void ResourceManager::release(ResourceBase* resource) {
        if(!resource)
            return;
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

    void ResourceManager::reset(const std::unordered_map<std::string, nlohmann::json>& pathMap) {
        this->pathMap = pathMap;
    }

    Texture2D* ResourceManager::loadTexture2D(const nlohmann::json& path) {
        return new Texture2D(path["path"].get<std::string>());
    }

    Sprite* ResourceManager::loadSprite(const std::string& identifier) {
        size_t pos1 = identifier.find(':');
        size_t pos2 = identifier.find('#');
        if(pos2 == std::string::npos) {
            // spr:tex:abc.png
            nlohmann::json data;
            data["texture"] = identifier.substr(pos1 + 1);
            data["texRegion"] = {0.0f, 0.0f, 1.0f, 1.0f};
            return new Sprite(data);
        }else {
            // spr:tex:123#a
            nlohmann::json data;
            data["texture"] = identifier.substr(pos1 + 1, pos2 - pos1 - 1);
            auto it = pathMap.find(identifier.substr(pos1 + 1, pos2 - pos1 - 1));
            CB_ASSERT(it != pathMap.end());
            data["texRegion"] = it->second["sprites"][identifier.substr(pos2 + 1)];
            return new Sprite(data);
        }
    }

    AnimationClip* ResourceManager::loadAnimationClip(const nlohmann::json& path) {
        return new AnimationClip(path["path"].get<std::string>());
    }

    Font* ResourceManager::loadFont(const nlohmann::json& path) {
        return new Font(path["fontFilePath"].get<std::string>(), path.value("fontSize", 16));
    }
}  // namespace Cube