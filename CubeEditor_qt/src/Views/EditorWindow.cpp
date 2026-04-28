#include "EditorWindow.h"

#include "AnimationEditorView.h"
#include "AssetBrowserView.h"
#include "AssetPropertyView.h"
#include "EntityPropertyView.h"
#include "SceneEditorView.h"
#include "SceneHierarchyView.h"

#include <QFileInfo>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QTabWidget>

EditorWindow::EditorWindow(const QString& projectFilePath, QWidget* parent)
    : QMainWindow(parent) {
    const QFileInfo info(projectFilePath);
    const QString projectName = info.completeBaseName().isEmpty() ? info.fileName() : info.completeBaseName();

    setWindowTitle("CubeEditor - " + projectName);
    resize(1360, 840);

    setupMenuBar();
    setupLayout(projectFilePath);
}

void EditorWindow::setupMenuBar() {
    auto* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("New Scene");
    auto* saveSceneAction = fileMenu->addAction("Save Scene");
    fileMenu->addSeparator();
    fileMenu->addAction("Exit");

    connect(saveSceneAction, &QAction::triggered, this, [this] {
        if(m_sceneHierarchyView) {
            m_sceneHierarchyView->saveSelectedScene();
        }
    });

    auto* editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction("Undo");
    editMenu->addAction("Redo");

    auto* viewMenu = menuBar()->addMenu("View");
    viewMenu->addAction("Reset Layout");

    auto* helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction("About");
}

void EditorWindow::setupLayout(const QString& projectFilePath) {
    auto* rootSplitter = new QSplitter(Qt::Horizontal, this);
    rootSplitter->setObjectName("rootSplitter");

    auto* leftTabs = new QTabWidget(rootSplitter);
    leftTabs->setObjectName("leftTabs");
    leftTabs->setTabsClosable(false);
    m_sceneHierarchyView = new SceneHierarchyView(projectFilePath, leftTabs);
    leftTabs->addTab(m_sceneHierarchyView, "Scene");

    auto* centerSplitter = new QSplitter(Qt::Vertical, rootSplitter);
    centerSplitter->setObjectName("centerSplitter");

    auto* workspaceTabs = new QTabWidget(centerSplitter);
    workspaceTabs->setObjectName("workspaceTabs");
    workspaceTabs->setTabsClosable(false);
    workspaceTabs->addTab(new SceneEditorView("Scene01", workspaceTabs), "Scene01");
    workspaceTabs->addTab(new SceneEditorView("Scene02", workspaceTabs), "Scene02");

    auto* bottomTabs = new QTabWidget(centerSplitter);
    bottomTabs->setObjectName("bottomTabs");
    bottomTabs->setTabsClosable(false);
    bottomTabs->addTab(new AssetBrowserView(bottomTabs), "Asset Browser");
    bottomTabs->addTab(new AnimationEditorView(bottomTabs), "Animation Editor");

    centerSplitter->addWidget(workspaceTabs);
    centerSplitter->addWidget(bottomTabs);
    centerSplitter->setStretchFactor(0, 4);
    centerSplitter->setStretchFactor(1, 2);
    centerSplitter->setSizes({560, 240});

    auto* rightTabs = new QTabWidget(rootSplitter);
    rightTabs->setObjectName("rightTabs");
    rightTabs->setTabsClosable(false);
    m_entityPropertyView = new EntityPropertyView(rightTabs);
    m_entityPropertyView->setSceneDirtyCallback([this](Cube::Scene* scene) {
        if(m_sceneHierarchyView) {
            m_sceneHierarchyView->markSceneDirty(scene);
        }
    });
    rightTabs->addTab(m_entityPropertyView, "Entity Property");
    rightTabs->addTab(new AssetPropertyView(rightTabs), "Asset Property");

    m_sceneHierarchyView->setSelectionChangedCallback([this](Cube::Scene* scene, Cube::Entity* entity) {
        if(m_entityPropertyView) {
            m_entityPropertyView->setSelection(scene, entity);
        }
    });

    rootSplitter->addWidget(leftTabs);
    rootSplitter->addWidget(centerSplitter);
    rootSplitter->addWidget(rightTabs);
    rootSplitter->setStretchFactor(0, 1);
    rootSplitter->setStretchFactor(1, 4);
    rootSplitter->setStretchFactor(2, 1);
    rootSplitter->setSizes({260, 860, 320});

    setCentralWidget(rootSplitter);
}
