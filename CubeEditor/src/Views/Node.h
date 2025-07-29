#pragma once

#include <string>
#include <vector>

namespace Cube {
    struct Node {
        std::string name = "Node";
        bool isGroup = false;
        bool isRenaming = false;
        bool justRenaming = false;
        std::vector<std::shared_ptr<Node>> children;
        std::weak_ptr<Node> parent;
    };
}