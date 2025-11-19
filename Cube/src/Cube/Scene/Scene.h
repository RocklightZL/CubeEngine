#pragma once
#include "Cube/Resource/Resource.h"

namespace Cube {
    class Node;

    class Scene : public ResourceBase {
    public:
        Scene(const std::string& path);
        ~Scene() = default;

        void update(float delta);

        static void createSceneFile(const Node* node, const std::string& filePath);

    private:
        std::unique_ptr<Node> rootNode = nullptr;
    };

}