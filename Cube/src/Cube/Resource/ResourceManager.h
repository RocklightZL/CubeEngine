#pragma once
#include "Atlas.h"

#include <json.hpp>

#include "Cube/Core/Log.h"
#include "Resource.h"

namespace Cube {
    class Texture2D;
    class Context;
    class Sprite;
    class AnimationClip;
    class Font;

    class ResourceManager {
    public:
        friend Context;

        // thread-local singleton
        static ResourceManager& get();
        static void init(const std::string& pathMapFilePath);
        static void init(const std::unordered_map<std::string, nlohmann::json>& pathMap);

        // Delete copy and move constructors and assignment operators
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

        // load
        template<typename T>
        T* load(const std::string& identifier) {
            static_assert(std::is_base_of_v<ResourceBase, T>);
            auto it = resourcesCache.find(identifier);
            if(it != resourcesCache.end()) {
                ResourceBase* resource = it->second.get();
                resource->refCount++;
                return static_cast<T*>(resource);
            }
            ResourceBase* newRes = nullptr;
            if constexpr (std::is_same_v<Sprite, T>) {
                newRes = loadSprite(identifier);
            } else {
                auto it2 = pathMap.find(identifier);
                if(it2 == pathMap.end()) {
                    CB_CORE_ERROR("Resource path not found: {}", identifier);
                    return nullptr;
                }
                if constexpr (std::is_same_v<Texture2D, T>) {
                    newRes = loadTexture2D(it2->second);
                }else if constexpr (std::is_same_v<AnimationClip, T>) {
                    newRes = loadAnimationClip(it2->second);
                }else if constexpr (std::is_same_v<Font, T>) {
                    newRes = loadFont(it2->second);
                }else {
                    static_assert(false);
                }
            }
            CB_ASSERT(newRes);
            newRes->refCount = 1;
            newRes->identifier = identifier;
            resourcesCache[identifier] = std::unique_ptr<ResourceBase>(newRes);
            return static_cast<T*>(newRes);
        }

        void release(ResourceBase* resource);
        void release(const std::string& identifier);
        void releaseAll();

        // for CubeEditor
        void reset(const std::unordered_map<std::string, nlohmann::json>& pathMap);

    protected:
        ResourceManager() = default;
        virtual ~ResourceManager() = default;

        std::unordered_map<std::string, std::unique_ptr<ResourceBase>> resourcesCache;
        std::unordered_map<std::string, nlohmann::json> pathMap;

    private:
        Texture2D* loadTexture2D(const nlohmann::json& path);
        Sprite* loadSprite(const std::string& identifier);
        AnimationClip* loadAnimationClip(const nlohmann::json& path);
        Font* loadFont(const nlohmann::json& path);
    };
}  // namespace Cube