#include "pch.h"

#include "SceneLayer.h"

namespace Cube {

    void SceneLayer::onUpdate(float deltaTime) {
        scene->onUpdate(deltaTime);
    }
}  // namespace Cube