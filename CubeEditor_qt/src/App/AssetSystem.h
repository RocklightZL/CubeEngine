#pragma once

#include <memory>
#include <string>
#include <vector>

namespace CubeEditor {

class AssetNode {
public:
    explicit AssetNode(std::string name);
    virtual ~AssetNode() = default;

    const std::string& getName() const;
    void setName(const std::string& n);
    AssetNode* getParent() const;
    void setParent(AssetNode* p);

    virtual bool isGroup() const = 0;

private:
    std::string name;
    AssetNode* parent = nullptr;
};

class AssetItem;

class AssetGroup : public AssetNode {
public:
    explicit AssetGroup(std::string name);
    ~AssetGroup() override = default;

    bool isGroup() const override { return true; }

    AssetGroup* addGroup(const std::string& groupName);
    AssetItem* addResource(const std::string& identifier);

    const std::vector<std::unique_ptr<AssetNode>>& getChildren() const;
    void clear();

private:
    std::vector<std::unique_ptr<AssetNode>> children;
};

class AssetItem : public AssetNode {
public:
    explicit AssetItem(std::string identifier);
    ~AssetItem() override = default;

    bool isGroup() const override { return false; }
    const std::string& getIdentifier() const;

private:
    std::string identifier;
};

class AssetSystem {
public:
    static AssetSystem& get();

    AssetGroup* getRoot();

    AssetGroup* createGroup(AssetGroup* parent, const std::string& name);
    AssetItem* createResource(AssetGroup* parent, const std::string& identifier);

    AssetItem* findResource(const std::string& identifier);

    void reset();

private:
    AssetSystem();

    AssetGroup root;
};

} // namespace CubeEditor
