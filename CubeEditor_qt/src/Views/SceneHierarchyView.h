#pragma once

#include <QWidget>

#include <functional>
#include <memory>
#include <vector>

#include "Cube/Scene/Scene.h"

class QPoint;
class QString;
class QTreeWidget;
class QTreeWidgetItem;

class SceneHierarchyView final : public QWidget {
public:
    explicit SceneHierarchyView(const QString& projectFilePath, QWidget* parent = nullptr);
    ~SceneHierarchyView() override;

    bool saveSelectedScene();
    void setSelectionChangedCallback(std::function<void(Cube::Scene*, Cube::Entity*)> callback);

private:
    void loadFromProject(const QString& projectFilePath);
    void refreshTree();
    void loadEditorState();
    void saveEditorState() const;
    void openSceneFromFile(const QString& sceneFilePath, bool showUiError = true, bool refreshAfterOpen = true);
    void onCustomContextMenuRequested(const QPoint& pos);
    void onCreateScene();
    void onCreateEntity(QTreeWidgetItem* sceneItem);
    void onOpenScene();
    void onSaveScene(QTreeWidgetItem* sceneItem);
    void saveSceneByIndex(int sceneIndex);
    void notifySelectionChanged();
    int sceneIndexFromItem(QTreeWidgetItem* item) const;
    QTreeWidgetItem* resolveSceneItem(QTreeWidgetItem* item) const;

    struct SceneDocument {
        std::unique_ptr<Cube::Scene> scene;
        QString filePath;
        bool dirty = false;
    };

    QString m_projectFilePath;
    QString m_scenesDirPath;
    QString m_editorStateFilePath;
    QTreeWidget* m_tree = nullptr;
    std::vector<SceneDocument> m_openScenes;
    std::function<void(Cube::Scene*, Cube::Entity*)> m_selectionChangedCallback;
};
