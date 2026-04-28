#include "AssetSystem.h"

#include "Cube/Core/Log.h"

#include <functional>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QStringList>

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

AssetGroup* AssetGroup::findGroup(const std::string& groupName) {
    for(const auto& child : children) {
        if(!child || !child->isGroup()) {
            continue;
        }
        if(child->getName() == groupName) {
            return static_cast<AssetGroup*>(child.get());
        }
    }
    return nullptr;
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

bool AssetSystem::loadFromProject(const QString& projectFilePath) {
    projectRootPath = QFileInfo(projectFilePath).absolutePath();
    assetsDirPath = QDir(projectRootPath).filePath("Assets");
    assetMapFilePath = QDir(projectRootPath).filePath("AssetMap.json");

    if(!QDir(assetsDirPath).exists()) {
        QDir().mkpath(assetsDirPath);
    }

    QString error;
    if(!loadAssetMap(&error)) {
        CB_EDITOR_WARN("Load AssetMap failed: {}", error.toStdString());
        return false;
    }

    rebuildTreeFromAssetMap();
    return true;
}

bool AssetSystem::importFile(const QString& filePath, QString* error) {
    if(projectRootPath.isEmpty()) {
        if(error) {
            *error = "Project not initialized.";
        }
        return false;
    }

    QString importPath = filePath;
    const QString normalizedFile = normalizePath(QFileInfo(filePath).absoluteFilePath()).toLower();
    const QString normalizedAssets = normalizePath(QFileInfo(assetsDirPath).absoluteFilePath()).toLower();
    if(!normalizedFile.startsWith(normalizedAssets + "/")) {
        const QFileInfo srcInfo(filePath);
        const QString targetPath = QDir(assetsDirPath).filePath(srcInfo.fileName());
        if(QFileInfo::exists(targetPath)) {
            if(error) {
                *error = "Target file already exists in Assets/.";
            }
            return false;
        }
        if(!QFile::copy(filePath, targetPath)) {
            if(error) {
                *error = "Failed to copy file into Assets/.";
            }
            return false;
        }
        importPath = targetPath;
    }

    QJsonObject config = defaultImportConfigFor(importPath, error);
    if(config.isEmpty()) {
        return false;
    }

    const std::string identifier = toIdentifier(importPath);
    if(identifier.empty()) {
        if(error) {
            *error = "Invalid identifier.";
        }
        return false;
    }

    if(assetMap.contains(QString::fromStdString(identifier))) {
        if(error) {
            *error = "Identifier already exists.";
        }
        return false;
    }

    assetMap.insert(QString::fromStdString(identifier), config);
    if(!saveAssetMap(error)) {
        assetMap.remove(QString::fromStdString(identifier));
        return false;
    }

    rebuildTreeFromAssetMap();
    return true;
}

bool AssetSystem::saveAssetMap(QString* error) const {
    if(assetMapFilePath.isEmpty()) {
        if(error) {
            *error = "AssetMap path is empty.";
        }
        return false;
    }

    QFile file(assetMapFilePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        if(error) {
            *error = "Cannot write AssetMap.json.";
        }
        return false;
    }

    const QJsonDocument doc(assetMap);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

const QString& AssetSystem::getProjectRootPath() const {
    return projectRootPath;
}

const QString& AssetSystem::getAssetsDirPath() const {
    return assetsDirPath;
}

const QString& AssetSystem::getAssetMapFilePath() const {
    return assetMapFilePath;
}

const QJsonObject& AssetSystem::getAssetMap() const {
    return assetMap;
}

bool AssetSystem::loadAssetMap(QString* error) {
    assetMap = QJsonObject();

    QFile file(assetMapFilePath);
    if(!file.exists()) {
        if(!saveAssetMap(error)) {
            return false;
        }
        return true;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        if(error) {
            *error = "Cannot read AssetMap.json.";
        }
        return false;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if(!doc.isObject()) {
        if(error) {
            *error = "AssetMap.json is not a JSON object.";
        }
        return false;
    }

    assetMap = doc.object();
    return true;
}

void AssetSystem::rebuildTreeFromAssetMap() {
    root.clear();

    const QStringList keys = assetMap.keys();
    for(const QString& key : keys) {
        const QString normalized = normalizePath(key);
        const QStringList segments = normalized.split('/', Qt::SkipEmptyParts);
        if(segments.isEmpty()) {
            continue;
        }

        std::vector<std::string> groupSegments;
        for(int i = 0; i < segments.size() - 1; ++i) {
            groupSegments.push_back(segments[i].toStdString());
        }

        AssetGroup* parent = ensureGroupPath(groupSegments);
        if(!parent) {
            parent = &root;
        }
        parent->addResource(key.toStdString());
    }
}

AssetGroup* AssetSystem::ensureGroupPath(const std::vector<std::string>& segments) {
    AssetGroup* current = &root;
    for(const auto& segment : segments) {
        if(segment.empty()) {
            continue;
        }
        AssetGroup* found = current->findGroup(segment);
        if(!found) {
            found = current->addGroup(segment);
        }
        current = found;
    }
    return current;
}

QString AssetSystem::normalizePath(const QString& path) {
    return QDir::fromNativeSeparators(path);
}

QJsonObject AssetSystem::defaultImportConfigFor(const QString& filePath, QString* error) const {
    const QString ext = QFileInfo(filePath).suffix().toLower();
    const QString relPath = normalizePath(QDir(projectRootPath).relativeFilePath(filePath));

    if(relPath.isEmpty()) {
        if(error) {
            *error = "Invalid asset path.";
        }
        return QJsonObject();
    }

    if(ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp" || ext == "tga" || ext == "dds") {
        QJsonObject config;
        config.insert("path", relPath);
        return config;
    }

    if(ext == "ttf" || ext == "otf") {
        QJsonObject config;
        config.insert("fontFilePath", relPath);
        config.insert("fontSize", 16);
        return config;
    }

    if(ext == "anim" || ext == "clip") {
        QJsonObject config;
        config.insert("path", relPath);
        return config;
    }

    if(error) {
        *error = "Unsupported asset type.";
    }
    return QJsonObject();
}

std::string AssetSystem::toIdentifier(const QString& filePath) {
    return QFileInfo(filePath).completeBaseName().toStdString();
}

} // namespace CubeEditor
