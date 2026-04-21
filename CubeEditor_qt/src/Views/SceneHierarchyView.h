#pragma once

#include <QWidget>

class QPoint;
class QString;
class QTreeWidget;
class QTreeWidgetItem;

class SceneHierarchyView final : public QWidget {
public:
    explicit SceneHierarchyView(const QString& projectFilePath, QWidget* parent = nullptr);

private:
    void loadFromProject(const QString& projectFilePath);
    void onCustomContextMenuRequested(const QPoint& pos);
    void onCreateScene();
    void onCreateEntity(QTreeWidgetItem* sceneItem);
    QTreeWidgetItem* resolveSceneItem(QTreeWidgetItem* item) const;

    QString m_projectFilePath;
    QString m_scenesDirPath;
    QTreeWidget* m_tree = nullptr;
};
