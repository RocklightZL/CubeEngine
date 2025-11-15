#include "pch.h"
#include "Node.h"

#include "Node2D.h"

namespace Cube {

    std::unordered_map<std::string, Node::CreateFunc> Node::constructorMap = {
        {"Node2D", []{ return new Node2D(); }}
    };

    nlohmann::json Node::serialize() const {
        nlohmann::json data;
        data["name"] = name;
        data["type"] = getType();
        return data;
    }

    void Node::deserialize(const nlohmann::json& data) {
        name = data["name"];
    }

    void Node::registerConstructor(const std::string& NodeType, const CreateFunc& func) {
        constructorMap.insert({NodeType, func});
    }

    Node* Node::createNode(const std::string& NodeType) {
        auto it = constructorMap.find(NodeType);
        if(it == constructorMap.end()) {
            CB_CORE_ERROR("Unknown Node type.");
            return nullptr;
        }
        return it->second();
    }
}  // namespace Cube