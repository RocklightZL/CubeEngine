#pragma once

#include <QMainWindow>

class SceneHierarchyView;
class EntityPropertyView;

class EditorWindow final : public QMainWindow {
public:
    explicit EditorWindow(const QString& projectFilePath, QWidget* parent = nullptr);

private:
    void setupMenuBar();
    void setupLayout(const QString& projectFilePath);

    SceneHierarchyView* m_sceneHierarchyView = nullptr;
    EntityPropertyView* m_entityPropertyView = nullptr;
};
