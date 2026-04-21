#include "GuidancePage.h"

#include "Cube/Core/Log.h"
#include "../App/ProjectRepository.h"
#include "NewProjectDialog.h"

#include <QAbstractItemView>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

GuidancePage::GuidancePage(std::shared_ptr<ProjectRepository> repository,
                           std::function<void(const QString&)> openEditorCallback,
                           QWidget* parent)
    : QWidget(parent)
    , m_repository(std::move(repository))
    , m_openEditorCallback(std::move(openEditorCallback)) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(14);

    auto* mainLayout = new QHBoxLayout();
    mainLayout->setSpacing(18);
    mainLayout->addWidget(buildLeftPanel(), 1);
    mainLayout->addWidget(buildRightPanel());
    root->addLayout(mainLayout, 1);

    setStyleSheet(R"(
        QWidget {
            background: #252526;
            color: #f3f3f3;
            font-size: 13px;
        }
        QFrame#recentPanel {
            border: 1px solid #3a3a3d;
            border-radius: 0px;
            background: #252526;
        }
        QListWidget#recentList {
            border: none;
            background: transparent;
            outline: none;
        }
        QListWidget#recentList::item {
            height: 30px;
            padding-left: 8px;
        }
        QListWidget#recentList::item:hover {
            background: #313136;
        }
        QListWidget#recentList::item:selected {
            background: #094771;
            color: #ffffff;
        }
        QPushButton#quickAction {
            border: 1px solid #3f3f46;
            border-radius: 3px;
            text-align: left;
            padding: 0 10px;
            color: #f3f3f3;
            background: #2d2d30;
            min-height: 32px;
        }
        QPushButton#quickAction:hover {
            border: 1px solid #83838a;
            background: #3a3a40;
        }
        QPushButton#quickAction:pressed {
            background: #1f1f24;
            border: 1px solid #6a6a70;
        }
    )");

    reloadRecentProjects();
}

QWidget* GuidancePage::buildLeftPanel() {
    auto* wrapper = new QWidget(this);
    auto* layout = new QVBoxLayout(wrapper);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* recentPanel = new QFrame(wrapper);
    recentPanel->setObjectName("recentPanel");

    auto* panelLayout = new QVBoxLayout(recentPanel);
    panelLayout->setContentsMargins(8, 8, 8, 8);
    panelLayout->setSpacing(0);

    m_recentList = new QListWidget(recentPanel);
    m_recentList->setObjectName("recentList");
    m_recentList->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(m_recentList, &QListWidget::itemClicked, this, &GuidancePage::onRecentProjectActivated);

    panelLayout->addWidget(m_recentList);
    layout->addWidget(recentPanel, 1);

    return wrapper;
}

QWidget* GuidancePage::buildRightPanel() {
    auto* wrapper = new QWidget(this);
    wrapper->setFixedWidth(290);

    auto* layout = new QVBoxLayout(wrapper);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    auto buildQuickAction = [wrapper](const QString& text) {
        auto* btn = new QPushButton(text, wrapper);
        btn->setObjectName("quickAction");
        return btn;
    };

    auto* newProjectBtn = buildQuickAction("[+] Create Project");
    auto* openProjectBtn = buildQuickAction("[P] Open Project");

    layout->addWidget(newProjectBtn);
    layout->addWidget(openProjectBtn);
    layout->addStretch(1);

    connect(newProjectBtn, &QPushButton::clicked, this, &GuidancePage::onCreateProject);
    connect(openProjectBtn, &QPushButton::clicked, this, &GuidancePage::onOpenProject);

    return wrapper;
}

void GuidancePage::reloadRecentProjects() {
    if(!m_recentList || !m_repository) {
        return;
    }

    m_recentList->clear();

    const QStringList recentProjects = m_repository->recentProjects();
    CB_EDITOR_INFO("Reload recent projects. Count={}", static_cast<int>(recentProjects.size()));
    for(const QString& projectPath : recentProjects) {
        QFileInfo info(projectPath);
        const QString displayName = info.completeBaseName().isEmpty() ? info.fileName() : info.completeBaseName();

        auto* item = new QListWidgetItem(displayName, m_recentList);
        item->setData(Qt::UserRole, projectPath);
        item->setToolTip(projectPath);
    }

    if(m_recentList->count() > 0) {
        m_recentList->setCurrentRow(0);
    }
}

void GuidancePage::onCreateProject() {
    if(!m_repository) {
        CB_EDITOR_ERROR("Create project action ignored: repository is null.");
        return;
    }

    NewProjectDialog dialog(this);
    if(dialog.exec() != QDialog::Accepted) {
        CB_EDITOR_INFO("Create project dialog canceled.");
        return;
    }

    const auto result = m_repository->createProject(dialog.projectName(), dialog.projectDirectory());
    if(!result.ok) {
        CB_EDITOR_WARN("Create project failed: {}", result.error.toStdString());
        QMessageBox::warning(this, "Create Project Failed", result.error);
        return;
    }

    CB_EDITOR_INFO("Create project succeeded: {}", result.projectFilePath.toStdString());
    reloadRecentProjects();
    if(m_openEditorCallback) {
        m_openEditorCallback(result.projectFilePath);
    }
}

void GuidancePage::onOpenProject() {
    if(!m_repository) {
        CB_EDITOR_ERROR("Open project action ignored: repository is null.");
        return;
    }

    const QString projectPath = QFileDialog::getOpenFileName(this,
                                                             "Open Project",
                                                             QString(),
                                                             "Cube Project File (*.cbproj)");
    if(projectPath.isEmpty()) {
        CB_EDITOR_INFO("Open project dialog canceled.");
        return;
    }

    if(!QFileInfo::exists(projectPath)) {
        CB_EDITOR_WARN("Selected project file does not exist: {}", projectPath.toStdString());
        QMessageBox::warning(this, "Open Project Failed", "The selected project file does not exist.");
        return;
    }

    m_repository->addRecentProject(projectPath);
    reloadRecentProjects();

    CB_EDITOR_INFO("Project opened from dialog: {}", projectPath.toStdString());
    if(m_openEditorCallback) {
        m_openEditorCallback(projectPath);
    }
}

void GuidancePage::onRecentProjectActivated(QListWidgetItem* item) {
    if(!item || !m_openEditorCallback) {
        return;
    }

    const QString projectPath = item->data(Qt::UserRole).toString();
    if(projectPath.isEmpty()) {
        CB_EDITOR_WARN("Recent project item missing path data.");
        return;
    }

    if(!QFileInfo::exists(projectPath)) {
        CB_EDITOR_WARN("Recent project file missing: {}", projectPath.toStdString());
        QMessageBox::warning(this, "Open Project Failed", "The selected recent project file does not exist anymore.");
        if(m_repository) {
            reloadRecentProjects();
        }
        return;
    }

    if(m_repository) {
        m_repository->addRecentProject(projectPath);
        reloadRecentProjects();
    }

    CB_EDITOR_INFO("Project opened from recent list: {}", projectPath.toStdString());
    m_openEditorCallback(projectPath);
}
