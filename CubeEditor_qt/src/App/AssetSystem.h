#pragma once

#include <memory>
#include <string>
#include <vector>

#include <QJsonObject>
#include <QString>

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
    AssetGroup* findGroup(const std::string& groupName);

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

    bool loadFromProject(const QString& projectFilePath);
    bool importFile(const QString& filePath, QString* error = nullptr);
    bool saveAssetMap(QString* error = nullptr) const;

    const QString& getProjectRootPath() const;
    const QString& getAssetsDirPath() const;
    const QString& getAssetMapFilePath() const;
    const QJsonObject& getAssetMap() const;

    void reset();

private:
    AssetSystem();
    bool loadAssetMap(QString* error);
    void rebuildTreeFromAssetMap();
    AssetGroup* ensureGroupPath(const std::vector<std::string>& segments);

    static QString normalizePath(const QString& path);
    QJsonObject defaultImportConfigFor(const QString& filePath, QString* error) const;
    static std::string toIdentifier(const QString& filePath);

    AssetGroup root;
    QJsonObject assetMap;
    QString projectRootPath;
    QString assetsDirPath;
    QString assetMapFilePath;
};

} // namespace CubeEditor
