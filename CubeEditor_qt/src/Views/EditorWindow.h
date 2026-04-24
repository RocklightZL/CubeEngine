#pragma once

#include <QMainWindow>

class SceneHierarchyView;

class EditorWindow final : public QMainWindow {
public:
    explicit EditorWindow(const QString& projectFilePath, QWidget* parent = nullptr);

private:
    void setupMenuBar();
    void setupLayout(const QString& projectFilePath);
    void setupStyle();

    SceneHierarchyView* m_sceneHierarchyView = nullptr;
};
