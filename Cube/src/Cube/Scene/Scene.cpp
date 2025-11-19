#include "pch.h"
#include "Scene.h"

#include "Node.h"

#include <fstream>
#include <json.hpp>

namespace Cube {

    Scene::Scene(const std::string& path) {
        std::ifstream file(path);
        if(!file.is_open()) {
            CB_CORE_ERROR("Failed to open scene resource file: {}", path);
            return;
        }
        nlohmann::json data;
        file >> data;
        file.close();
        rootNode.reset(Node::createNode(data["type"]));
        rootNode->deserialize(data);
    }

    void Scene::update(float delta) {
        rootNode->update(delta);
    }

    void Scene::createSceneFile(const Node* node, const std::string& filePath) {
        std::ofstream file(filePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("Failed to create scene resource file: {}", filePath);
            return;
        }
        nlohmann::json data = node->serialize();
        file << data.dump(4);
        file.close();
    }

}  // namespace Cube