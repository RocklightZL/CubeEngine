#include "pch.h"
#include "SceneLayer.h"

#include "Cube/Scene/Scene.h"
#include "Cube/Resource/ResourceManager.h"

namespace Cube {

    SceneLayer::SceneLayer(const std::string& path) {
        scene = ResourceManager::get().load<Scene>(path);
    }

    SceneLayer::~SceneLayer() {
        ResourceManager::get().release(scene);
    }

    void SceneLayer::onUpdate(float deltaTime) {
        scene->update(deltaTime);
    }

}  // namespace Cube