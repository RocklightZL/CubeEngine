#include "AssetExplorer.h"

#include "Cube/Core/Log.h"

#include <fstream>

void AssetExplorer::normalInit() {
    rootNode = std::make_unique<AssetNode>();
    rootNode->name = "root";
    rootNode->isGroup = true;
}

void AssetExplorer::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if(!file.is_open()) {
        CB_EDITOR_ERROR("Failed to open file {}", path);
        return;
    }
    nlohmann::json data;
    file >> data;
    file.close();
    rootNode = std::make_unique<AssetNode>();
    rootNode->fromJson(data);
}

void AssetExplorer::saveToFile(const std::string& path) const {
    std::ofstream file(path);
    if(!file.is_open()) {
        CB_EDITOR_ERROR("Failed to open file {}", path);
        return;
    }
    file << rootNode->toJson().dump(4);
}

AssetNode* AssetExplorer::getCurrentNode() const {
    return nodeStack.empty() ? rootNode.get() : nodeStack.top();
}

void AssetExplorer::enterNode(AssetNode* node) {
    if(node->isGroup) {
        nodeStack.push(node);
        currentPath += node->name + "/";
    }
}

void AssetExplorer::back() {
    if(!nodeStack.empty()) {
        nodeStack.pop();
        size_t pos = currentPath.find_last_of('/', currentPath.length() - 2);
        currentPath = currentPath.substr(0, pos + 1);
    }
}