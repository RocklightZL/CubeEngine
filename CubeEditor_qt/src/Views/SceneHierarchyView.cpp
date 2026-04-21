#include "SceneHierarchyView.h"

#include "Cube/Core/Log.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTreeWidget>
#include <QVBoxLayout>

SceneHierarchyView::SceneHierarchyView(const QString& projectFilePath, QWidget* parent)
    : QWidget(parent)
    , m_projectFilePath(projectFilePath) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(0);

    m_tree = new QTreeWidget(this);
    m_tree->setObjectName("sceneHierarchyTree");
    m_tree->setHeaderHidden(true);
    m_tree->setIndentation(14);
    m_tree->setRootIsDecorated(true);
    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    m_tree->setStyleSheet(R"(
        QTreeWidget#sceneHierarchyTree {
            border: none;
            background: transparent;
            outline: none;
            color: #f3f3f3;
        }
        QTreeWidget#sceneHierarchyTree::item {
            height: 26px;
        }
        QTreeWidget#sceneHierarchyTree::item:hover {
            background: #313136;
        }
        QTreeWidget#sceneHierarchyTree::item:selected {
            background: #094771;
            color: #ffffff;
        }
    )");

    connect(m_tree, &QTreeWidget::customContextMenuRequested, this, &SceneHierarchyView::onCustomContextMenuRequested);

    layout->addWidget(m_tree);

    loadFromProject(projectFilePath);
}

void SceneHierarchyView::loadFromProject(const QString& projectFilePath) {
    CB_EDITOR_INFO("Load scene hierarchy from project: {}", projectFilePath.toStdString());

    m_tree->clear();

    const QDir projectRoot(QFileInfo(projectFilePath).absolutePath());
    m_scenesDirPath = projectRoot.filePath("Scenes");

    QDir scenesDir(m_scenesDirPath);
    if(!scenesDir.exists()) {
        QDir().mkpath(m_scenesDirPath);
        scenesDir = QDir(m_scenesDirPath);
    }

    const QFileInfoList sceneFiles = scenesDir.entryInfoList(QStringList() << "*.scene", QDir::Files | QDir::NoSymLinks, QDir::Name);

    for(const QFileInfo& sceneFile : sceneFiles) {
        QFile file(sceneFile.absoluteFilePath());
        QString sceneDisplayName = sceneFile.baseName();

        QJsonArray entities;
        if(file.open(QIODevice::ReadOnly)) {
            const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if(doc.isObject()) {
                const QJsonObject root = doc.object();
                const QString jsonSceneName = root.value("name").toString().trimmed();
                if(!jsonSceneName.isEmpty()) {
                    sceneDisplayName = jsonSceneName;
                }
                entities = root.value("entities").toArray();
            } else {
                CB_EDITOR_WARN("Scene file has invalid JSON root object: {}", sceneFile.absoluteFilePath().toStdString());
            }
            file.close();
        } else {
            CB_EDITOR_WARN("Cannot open scene file: {}", sceneFile.absoluteFilePath().toStdString());
        }

        auto* sceneItem = new QTreeWidgetItem(m_tree);
        sceneItem->setText(0, sceneDisplayName);
        sceneItem->setData(0, Qt::UserRole, sceneFile.absoluteFilePath());

        for(const QJsonValue& entityValue : entities) {
            if(!entityValue.isObject()) {
                continue;
            }
            const QString entityName = entityValue.toObject().value("name").toString().trimmed();
            auto* entityItem = new QTreeWidgetItem(sceneItem);
            entityItem->setText(0, entityName.isEmpty() ? QStringLiteral("<Unnamed Entity>") : entityName);
        }

        sceneItem->setExpanded(true);
    }

    if(m_tree->topLevelItemCount() == 0) {
        auto* emptyItem = new QTreeWidgetItem(m_tree);
        emptyItem->setText(0, "No scene files found in Scenes/");
        emptyItem->setData(0, Qt::UserRole, QString());
    }
}

void SceneHierarchyView::onCustomContextMenuRequested(const QPoint& pos) {
    QTreeWidgetItem* clickedItem = m_tree->itemAt(pos);
    QTreeWidgetItem* sceneItem = resolveSceneItem(clickedItem);

    QMenu menu(this);
    QAction* createSceneAction = menu.addAction("New Scene");
    QAction* createEntityAction = nullptr;

    if(sceneItem) {
        createEntityAction = menu.addAction("New Entity");
    }

    QAction* selected = menu.exec(m_tree->viewport()->mapToGlobal(pos));
    if(selected == createSceneAction) {
        onCreateScene();
        return;
    }
    if(createEntityAction && selected == createEntityAction) {
        onCreateEntity(sceneItem);
    }
}

void SceneHierarchyView::onCreateScene() {
    bool ok = false;
    const QString sceneName = QInputDialog::getText(this,
                                                    "New Scene",
                                                    "Scene Name:",
                                                    QLineEdit::Normal,
                                                    QString(),
                                                    &ok).trimmed();
    if(!ok) {
        return;
    }
    if(sceneName.isEmpty()) {
        QMessageBox::warning(this, "New Scene", "Scene name cannot be empty.");
        return;
    }

    static const QRegularExpression invalidPattern(R"([\\/:*?"<>|])");
    if(sceneName.contains(invalidPattern)) {
        QMessageBox::warning(this, "New Scene", "Scene name contains invalid file characters.");
        return;
    }

    const QString sceneFilePath = QDir(m_scenesDirPath).filePath(sceneName + ".scene");
    if(QFileInfo::exists(sceneFilePath)) {
        QMessageBox::warning(this, "New Scene", "A scene file with the same name already exists.");
        return;
    }

    QJsonObject root;
    root.insert("name", sceneName);
    root.insert("entities", QJsonArray());

    QFile file(sceneFilePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        CB_EDITOR_ERROR("Create scene failed: cannot open file {}", sceneFilePath.toStdString());
        QMessageBox::warning(this, "New Scene", "Failed to create scene file.");
        return;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.close();

    CB_EDITOR_INFO("Scene created: {}", sceneFilePath.toStdString());
    loadFromProject(m_projectFilePath);
}

void SceneHierarchyView::onCreateEntity(QTreeWidgetItem* sceneItem) {
    if(!sceneItem) {
        return;
    }

    const QString sceneFilePath = sceneItem->data(0, Qt::UserRole).toString();
    if(sceneFilePath.isEmpty()) {
        return;
    }

    bool ok = false;
    const QString entityName = QInputDialog::getText(this,
                                                     "New Entity",
                                                     "Entity Name:",
                                                     QLineEdit::Normal,
                                                     QString(),
                                                     &ok).trimmed();
    if(!ok) {
        return;
    }
    if(entityName.isEmpty()) {
        QMessageBox::warning(this, "New Entity", "Entity name cannot be empty.");
        return;
    }

    QFile file(sceneFilePath);
    if(!file.open(QIODevice::ReadOnly)) {
        CB_EDITOR_ERROR("Create entity failed: cannot open scene {}", sceneFilePath.toStdString());
        QMessageBox::warning(this, "New Entity", "Failed to open scene file.");
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if(!doc.isObject()) {
        CB_EDITOR_ERROR("Create entity failed: invalid scene JSON {}", sceneFilePath.toStdString());
        QMessageBox::warning(this, "New Entity", "Scene file format is invalid.");
        return;
    }

    QJsonObject root = doc.object();
    QJsonArray entities = root.value("entities").toArray();

    for(const QJsonValue& entityValue : entities) {
        if(entityValue.isObject() && entityValue.toObject().value("name").toString() == entityName) {
            QMessageBox::warning(this, "New Entity", "An entity with the same name already exists in this scene.");
            return;
        }
    }

    QJsonObject entity;
    entity.insert("name", entityName);

    QJsonObject transform;
    transform.insert("pos", QJsonArray{0.0, 0.0});
    transform.insert("rotation", 0.0);
    transform.insert("scale", QJsonArray{1.0, 1.0});
    transform.insert("children", QJsonArray());

    entity.insert("transform", transform);
    entity.insert("components", QJsonArray());

    entities.append(entity);
    root.insert("entities", entities);

    QFile outFile(sceneFilePath);
    if(!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        CB_EDITOR_ERROR("Create entity failed: cannot write scene {}", sceneFilePath.toStdString());
        QMessageBox::warning(this, "New Entity", "Failed to write scene file.");
        return;
    }

    outFile.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    outFile.close();

    CB_EDITOR_INFO("Entity created: {} in {}", entityName.toStdString(), sceneFilePath.toStdString());
    loadFromProject(m_projectFilePath);
}

QTreeWidgetItem* SceneHierarchyView::resolveSceneItem(QTreeWidgetItem* item) const {
    if(!item) {
        return nullptr;
    }

    QTreeWidgetItem* sceneItem = item->parent() ? item->parent() : item;
    const QString sceneFilePath = sceneItem->data(0, Qt::UserRole).toString();
    if(sceneFilePath.isEmpty()) {
        return nullptr;
    }
    return sceneItem;
}
