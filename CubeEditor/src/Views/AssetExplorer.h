#pragma once

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
        if(isGroup) {
            json["children"] = nlohmann::json::array();
            for(auto& c : children) {
                json["children"].push_back(c->toJson());
            }
        }else {
            json["identifier"] = identifier;
        }
        return json;
    }

    void fromJson(const nlohmann::json& json) {
        name = json["name"];
        isGroup = json["isGroup"];
        if(isGroup) {
            for(auto& c : json["children"]) {
                std::unique_ptr<AssetNode> n = std::make_unique<AssetNode>();
                n->fromJson(c);
                children.push_back(std::move(n));
            }
        }else {
            identifier = json["identifier"];
            type = Cube::getResType(identifier);
        }
    }
};

class AssetExplorer {
public:
    AssetExplorer() = default;
    ~AssetExplorer() = default;

    void normalInit();
    void loadFromFile(const std::string& path, const std::string& assetMapFilePath);
    void saveToFile(const std::string& path, const std::string& assetMapFilePath) const;
    AssetNode* getCurrentNode() const;
    void enterNode(AssetNode* node);
    void back();
    void addNode(AssetNode* node);
    AssetNode* createGroup(const std::string& name);
    void createResource(const std::string& identifier, const nlohmann::json& content);
    void removeNode(AssetNode* node);
    void move(const AssetNode* src, AssetNode* dst);

    const std::string& getCurrentPath() const { return currentPath; }
    const std::unordered_map<std::string, nlohmann::json>& getAssetPathMap() const { return assetPathMap; }

private:
    std::unique_ptr<AssetNode> rootNode;
    std::stack<AssetNode*> nodeStack;
    std::string currentPath = "/";

    std::unordered_map<std::string, nlohmann::json> assetPathMap;

    void resetResourceManager();
    void _removeNode(AssetNode* node);
};
