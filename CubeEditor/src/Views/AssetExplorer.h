#pragma once

#include "Cube/Resource/ResourceType.h"
#include "Cube/Resource/ResourceType.h"

#include <json.hpp>
#include <stack>
#include <string>
#include <vector>

struct AssetNode {
    std::string name = "AssetNode";
    std::string identifier;
    Cube::ResourceType type = Cube::ResourceType::Unknown;
    bool isGroup = false;
    std::vector<std::unique_ptr<AssetNode>> children;

    nlohmann::json toJson() const {
        nlohmann::json json;
        json["name"] = name;
        json["isGroup"] = isGroup;
        json["children"] = nlohmann::json::array();
        for(auto& c : children) {
            json["children"].push_back(c->toJson());
        }
        return json;
    }

    void fromJson(const nlohmann::json& json) {
        name = json["name"];
        isGroup = json["isGroup"];
        if(!isGroup) {
            type = Cube::getResType(name);
            identifier = name;
        }
        for(auto& c : json["children"]) {
            std::unique_ptr<AssetNode> n = std::make_unique<AssetNode>();
            n->fromJson(c);
            children.push_back(std::move(n));
        }
    }
};

class AssetExplorer {
public:
    AssetExplorer() = default;
    ~AssetExplorer() = default;

    void normalInit();
    void loadFromFile(const std::string& path);
    void saveToFile(const std::string& path) const;
    AssetNode* getCurrentNode() const;
    void enterNode(AssetNode* node);
    void back();
    void addNode(AssetNode* node);

    const std::string& getCurrentPath() const { return currentPath; }

private:
    std::unique_ptr<AssetNode> rootNode;
    std::stack<AssetNode*> nodeStack;
    std::string currentPath = "/";
};
