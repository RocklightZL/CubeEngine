#include "AssetSystem.h"

#include <functional>

namespace CubeEditor {

AssetNode::AssetNode(std::string name) : name(std::move(name)) {}

const std::string& AssetNode::getName() const {
    return name;
}

void AssetNode::setName(const std::string& n) {
    name = n;
}

AssetNode* AssetNode::getParent() const {
    return parent;
}

void AssetNode::setParent(AssetNode* p) {
    parent = p;
}

AssetGroup::AssetGroup(std::string name) : AssetNode(std::move(name)) {}

AssetGroup* AssetGroup::addGroup(const std::string& groupName) {
    auto node = std::make_unique<AssetGroup>(groupName);
    node->setParent(this);
    AssetGroup* ptr = node.get();
    children.emplace_back(std::move(node));
    return ptr;
}

AssetItem* AssetGroup::addResource(const std::string& identifier) {
    auto node = std::make_unique<AssetItem>(identifier);
    node->setParent(this);
    AssetItem* ptr = node.get();
    children.emplace_back(std::move(node));
    return ptr;
}

const std::vector<std::unique_ptr<AssetNode>>& AssetGroup::getChildren() const {
    return children;
}

void AssetGroup::clear() {
    children.clear();
}

AssetItem::AssetItem(std::string identifier)
    : AssetNode(identifier),
      identifier(std::move(identifier)) {}

const std::string& AssetItem::getIdentifier() const {
    return identifier;
}

AssetSystem& AssetSystem::get() {
    static AssetSystem s;
    return s;
}

AssetSystem::AssetSystem()
    : root("Assets") {}

AssetGroup* AssetSystem::getRoot() {
    return &root;
}

AssetGroup* AssetSystem::createGroup(AssetGroup* parent, const std::string& name) {
    if(!parent) {
        parent = &root;
    }
    return parent->addGroup(name);
}

AssetItem* AssetSystem::createResource(AssetGroup* parent, const std::string& identifier) {
    if(!parent) {
        parent = &root;
    }
    return parent->addResource(identifier);
}

AssetItem* AssetSystem::findResource(const std::string& identifier) {
    std::function<AssetItem*(AssetNode*)> dfs = [&](AssetNode* node) -> AssetItem* {
        if(!node) {
            return nullptr;
        }
        if(!node->isGroup()) {
            auto* item = static_cast<AssetItem*>(node);
            return item->getIdentifier() == identifier ? item : nullptr;
        }
        auto* group = static_cast<AssetGroup*>(node);
        for(const auto& child : group->getChildren()) {
            AssetItem* found = dfs(child.get());
            if(found) {
                return found;
            }
        }
        return nullptr;
    };

    return dfs(&root);
}

void AssetSystem::reset() {
    root.clear();
}

} // namespace CubeEditor
