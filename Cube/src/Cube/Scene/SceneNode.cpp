#include "pch.h"
#include "SceneNode.h"

#include "Cube/Resource/ResourceManager.h"

namespace Cube {

    SceneNode::~SceneNode() {
        if(scene) {
            ResourceManager::release(scene);
        }
    }

    void SceneNode::update(float delta) {
        if(scene) {
            scene->update(delta);
        }
    }

    nlohmann::json SceneNode::serialize() const {
        nlohmann::json data = Node::serialize();
        data["scene"] = scene ? scene->getPath() : "";
        return data;
    }

    void SceneNode::deserialize(const nlohmann::json& data) {
        Node::deserialize(data);
        std::string scenePath = data["scene"];
        if(!scenePath.empty()) {
            scene = ResourceManager::get().load<Scene>(scenePath);
        } else {
            scene = nullptr;
        }
    }
}  // namespace Cube