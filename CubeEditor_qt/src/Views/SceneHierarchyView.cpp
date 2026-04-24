#include "SceneHierarchyView.h"

#include "Cube/Core/Log.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace {
constexpr int kRoleSceneIndex = Qt::UserRole + 1;

QString normalizePath(const QString& path) {
    return QDir::fromNativeSeparators(QFileInfo(path).absoluteFilePath());
}
} // namespace

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

SceneHierarchyView::~SceneHierarchyView() {
    saveEditorState();
}

void SceneHierarchyView::loadFromProject(const QString& projectFilePath) {
    CB_EDITOR_INFO("Initialize scene hierarchy for project: {}", projectFilePath.toStdString());

    const QDir projectRoot(QFileInfo(projectFilePath).absolutePath());
    m_scenesDirPath = projectRoot.filePath("Scenes");
    const QString cubeDirPath = projectRoot.filePath(".cube");
    QDir().mkpath(cubeDirPath);
    m_editorStateFilePath = QDir(cubeDirPath).filePath("editor_state.json");

    QDir scenesDir(m_scenesDirPath);
    if(!scenesDir.exists()) {
        QDir().mkpath(m_scenesDirPath);
    }

    m_openScenes.clear();
    loadEditorState();
    refreshTree();
}

void SceneHierarchyView::loadEditorState() {
    QFile file(m_editorStateFilePath);
    if(!file.exists()) {
        CB_EDITOR_INFO("No editor state cache found: {}", m_editorStateFilePath.toStdString());
        return;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        CB_EDITOR_WARN("Cannot read editor state cache: {}", m_editorStateFilePath.toStdString());
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if(!doc.isObject()) {
        CB_EDITOR_WARN("Editor state cache has invalid JSON format: {}", m_editorStateFilePath.toStdString());
        return;
    }

    const QJsonObject root = doc.object();
    const QJsonArray openedScenes = root.value("openedScenes").toArray();
    const QString selectedScenePath = normalizePath(root.value("selectedScene").toString());

    for(const QJsonValue& value : openedScenes) {
        if(!value.isString()) {
            continue;
        }
        openSceneFromFile(value.toString(), false, false);
    }

    if(!selectedScenePath.isEmpty()) {
        for(int i = 0; i < static_cast<int>(m_openScenes.size()); ++i) {
            if(normalizePath(m_openScenes[i].filePath) == selectedScenePath) {
                refreshTree();
                QTreeWidgetItem* item = m_tree->topLevelItem(i);
                if(item) {
                    m_tree->setCurrentItem(item);
                }
                CB_EDITOR_INFO("Restored selected scene: {}", selectedScenePath.toStdString());
                return;
            }
        }
    }

    CB_EDITOR_INFO("Restored {} opened scenes from cache.", static_cast<int>(m_openScenes.size()));
}

void SceneHierarchyView::saveEditorState() const {
    if(m_editorStateFilePath.isEmpty()) {
        return;
    }

    QJsonArray openedScenes;
    for(const SceneDocument& scene : m_openScenes) {
        if(!scene.filePath.isEmpty()) {
            openedScenes.append(normalizePath(scene.filePath));
        }
    }

    QString selectedScenePath;
    QTreeWidgetItem* current = m_tree ? m_tree->currentItem() : nullptr;
    if(current) {
        const int index = sceneIndexFromItem(resolveSceneItem(current));
        if(index >= 0 && index < static_cast<int>(m_openScenes.size())) {
            selectedScenePath = normalizePath(m_openScenes[index].filePath);
        }
    }

    QJsonObject root;
    root.insert("openedScenes", openedScenes);
    root.insert("selectedScene", selectedScenePath);

    QFile file(m_editorStateFilePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        CB_EDITOR_WARN("Cannot write editor state cache: {}", m_editorStateFilePath.toStdString());
        return;
    }

    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.close();
}

void SceneHierarchyView::refreshTree() {
    m_tree->clear();

    for(size_t i = 0; i < m_openScenes.size(); ++i) {
        SceneDocument& scene = m_openScenes[i];
        if(!scene.scene) {
            continue;
        }

        auto* sceneItem = new QTreeWidgetItem(m_tree);
        const QString sceneName = QString::fromStdString(scene.scene->getName());
        sceneItem->setText(0, scene.dirty ? (sceneName + " *") : sceneName);
        sceneItem->setData(0, kRoleSceneIndex, static_cast<int>(i));
        sceneItem->setData(0, Qt::UserRole, scene.filePath);

        const auto& entities = scene.scene->getAllEntities();
        for(const auto& entity : entities) {
            auto* entityItem = new QTreeWidgetItem(sceneItem);
            const QString entityName = QString::fromStdString(entity->getName());
            entityItem->setText(0, entityName.isEmpty() ? QStringLiteral("<Unnamed Entity>") : entityName);
            entityItem->setData(0, kRoleSceneIndex, static_cast<int>(i));
        }

        sceneItem->setExpanded(true);
    }

    if(m_tree->topLevelItemCount() == 0) {
        auto* hintItem = new QTreeWidgetItem(m_tree);
        hintItem->setText(0, "No scene opened. Right click to open or create a scene.");
        hintItem->setData(0, kRoleSceneIndex, -1);
    }
}

void SceneHierarchyView::openSceneFromFile(const QString& sceneFilePath, bool showUiError, bool refreshAfterOpen) {
    const QString normalized = normalizePath(sceneFilePath);
    for(size_t i = 0; i < m_openScenes.size(); ++i) {
        if(!m_openScenes[i].filePath.isEmpty() && normalizePath(m_openScenes[i].filePath) == normalized) {
            CB_EDITOR_INFO("Skip opening scene: already opened {}", normalized.toStdString());
            if(refreshAfterOpen) {
                refreshTree();
                QTreeWidgetItem* sceneItem = m_tree->topLevelItem(static_cast<int>(i));
                if(sceneItem) {
                    m_tree->setCurrentItem(sceneItem);
                }
            }
            return;
        }
    }

    if(!QFileInfo::exists(normalized)) {
        CB_EDITOR_ERROR("Open scene failed: cannot open file {}", normalized.toStdString());
        if(showUiError) {
            QMessageBox::warning(this, "Open Scene", "Failed to open scene file.");
        }
        return;
    }

    SceneDocument scene;
    scene.filePath = normalized;
    scene.scene = std::make_unique<Cube::Scene>(normalized.toStdString());
    if(!scene.scene || scene.scene->getName().empty()) {
        CB_EDITOR_ERROR("Open scene failed: Scene object load returned empty scene name {}", normalized.toStdString());
        if(showUiError) {
            QMessageBox::warning(this, "Open Scene", "Failed to deserialize scene file.");
        }
        return;
    }
    scene.dirty = false;

    CB_EDITOR_INFO("Scene opened: {}", normalized.toStdString());
    m_openScenes.push_back(std::move(scene));
    if(refreshAfterOpen) {
        refreshTree();
        m_tree->setCurrentItem(m_tree->topLevelItem(m_tree->topLevelItemCount() - 1));
    }
    saveEditorState();
}

void SceneHierarchyView::onCustomContextMenuRequested(const QPoint& pos) {
    QTreeWidgetItem* clickedItem = m_tree->itemAt(pos);
    QTreeWidgetItem* sceneItem = resolveSceneItem(clickedItem);

    QMenu menu(this);
    QAction* openSceneAction = menu.addAction("Open Scene...");
    QAction* createSceneAction = menu.addAction("New Scene");
    QAction* createEntityAction = nullptr;
    QAction* saveSceneAction = nullptr;

    if(sceneItem) {
        createEntityAction = menu.addAction("New Entity");
        saveSceneAction = menu.addAction("Save Scene");
    }

    QAction* selected = menu.exec(m_tree->viewport()->mapToGlobal(pos));
    if(selected == openSceneAction) {
        onOpenScene();
        return;
    }
    if(selected == createSceneAction) {
        onCreateScene();
        return;
    }
    if(createEntityAction && selected == createEntityAction) {
        onCreateEntity(sceneItem);
        return;
    }
    if(saveSceneAction && selected == saveSceneAction) {
        onSaveScene(sceneItem);
    }
}

void SceneHierarchyView::onOpenScene() {
    const QString scenePath = QFileDialog::getOpenFileName(this,
                                                           "Open Scene",
                                                           m_scenesDirPath,
                                                           "Scene File (*.scene)");
    if(scenePath.isEmpty()) {
        return;
    }

    openSceneFromFile(scenePath);
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

    for(const SceneDocument& scene : m_openScenes) {
        if(scene.scene && QString::fromStdString(scene.scene->getName()) == sceneName && scene.filePath.isEmpty()) {
            QMessageBox::warning(this, "New Scene", "An unsaved scene with the same name is already opened.");
            return;
        }
    }

    SceneDocument scene;
    scene.scene = std::make_unique<Cube::Scene>(sceneName.toStdString(), true);
    scene.filePath = QString();
    scene.dirty = true;

    CB_EDITOR_INFO("New scene created in memory: {}", sceneName.toStdString());
    m_openScenes.push_back(std::move(scene));
    refreshTree();
    m_tree->setCurrentItem(m_tree->topLevelItem(m_tree->topLevelItemCount() - 1));
    saveEditorState();
}

void SceneHierarchyView::onCreateEntity(QTreeWidgetItem* sceneItem) {
    if(!sceneItem) {
        return;
    }

    const int sceneIndex = sceneIndexFromItem(sceneItem);
    if(sceneIndex < 0 || sceneIndex >= static_cast<int>(m_openScenes.size())) {
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

    SceneDocument& scene = m_openScenes[sceneIndex];
    const auto& allEntities = scene.scene->getAllEntities();
    for(const auto& entity : allEntities) {
        if(QString::fromStdString(entity->getName()) == entityName) {
            QMessageBox::warning(this, "New Entity", "An entity with the same name already exists in this scene.");
            return;
        }
    }

    scene.scene->createEntity(entityName.toStdString());
    scene.dirty = true;

    CB_EDITOR_INFO("Entity created in scene {}: {}", scene.scene->getName(), entityName.toStdString());
    refreshTree();
    saveEditorState();
}

void SceneHierarchyView::onSaveScene(QTreeWidgetItem* sceneItem) {
    if(!sceneItem) {
        return;
    }

    const int sceneIndex = sceneIndexFromItem(sceneItem);
    if(sceneIndex < 0) {
        return;
    }

    saveSceneByIndex(sceneIndex);
}

bool SceneHierarchyView::saveSelectedScene() {
    QTreeWidgetItem* current = m_tree->currentItem();
    QTreeWidgetItem* sceneItem = resolveSceneItem(current);
    if(!sceneItem) {
        QMessageBox::information(this, "Save Scene", "Please select a scene first.");
        return false;
    }

    const int sceneIndex = sceneIndexFromItem(sceneItem);
    if(sceneIndex < 0) {
        return false;
    }

    saveSceneByIndex(sceneIndex);
    return true;
}

void SceneHierarchyView::saveSceneByIndex(int sceneIndex) {
    if(sceneIndex < 0 || sceneIndex >= static_cast<int>(m_openScenes.size())) {
        return;
    }

    SceneDocument& scene = m_openScenes[sceneIndex];

    QString targetFilePath = scene.filePath;
    if(targetFilePath.isEmpty()) {
        const QString defaultSceneName = scene.scene ? QString::fromStdString(scene.scene->getName()) : QStringLiteral("Untitled");
        targetFilePath = QFileDialog::getSaveFileName(this,
                                                      "Save Scene",
                                                      QDir(m_scenesDirPath).filePath(defaultSceneName + ".scene"),
                                                      "Scene File (*.scene)");
        if(targetFilePath.isEmpty()) {
            return;
        }
        if(!targetFilePath.endsWith(".scene", Qt::CaseInsensitive)) {
            targetFilePath += ".scene";
        }

        const QString normalized = normalizePath(targetFilePath);
        for(size_t i = 0; i < m_openScenes.size(); ++i) {
            if(static_cast<int>(i) == sceneIndex) {
                continue;
            }
            if(!m_openScenes[i].filePath.isEmpty() && normalizePath(m_openScenes[i].filePath) == normalized) {
                QMessageBox::warning(this, "Save Scene", "Another opened scene already uses this file path.");
                return;
            }
        }

        targetFilePath = normalized;
    }

    if(!scene.scene) {
        CB_EDITOR_ERROR("Save scene failed: null Scene object.");
        QMessageBox::warning(this, "Save Scene", "Current scene is invalid.");
        return;
    }

    scene.scene->serialize(targetFilePath.toStdString());

    scene.filePath = targetFilePath;
    scene.dirty = false;

    CB_EDITOR_INFO("Scene saved: {}", targetFilePath.toStdString());
    refreshTree();
    saveEditorState();
}

int SceneHierarchyView::sceneIndexFromItem(QTreeWidgetItem* item) const {
    if(!item) {
        return -1;
    }

    bool ok = false;
    const int index = item->data(0, kRoleSceneIndex).toInt(&ok);
    if(!ok) {
        return -1;
    }
    return index;
}

QTreeWidgetItem* SceneHierarchyView::resolveSceneItem(QTreeWidgetItem* item) const {
    if(!item) {
        return nullptr;
    }

    QTreeWidgetItem* sceneItem = item->parent() ? item->parent() : item;
    const int index = sceneIndexFromItem(sceneItem);
    if(index < 0 || index >= static_cast<int>(m_openScenes.size())) {
        return nullptr;
    }
    return sceneItem;
}
