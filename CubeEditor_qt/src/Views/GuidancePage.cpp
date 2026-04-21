#include "GuidancePage.h"

#include "NewProjectDialog.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

GuidancePage::GuidancePage(QWidget* parent)
    : QWidget(parent) {
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

    auto* recentList = new QListWidget(recentPanel);
    recentList->setObjectName("recentList");
    recentList->addItem("CubeEngine");
    recentList->addItem("MSBuild.sln");
    recentList->addItem("freetype-2.14.3");
    recentList->addItem("CMakeLists.txt");
    recentList->setSelectionMode(QAbstractItemView::SingleSelection);
    recentList->setCurrentRow(0);

    panelLayout->addWidget(recentList);
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

    auto* newProjectBtn = buildQuickAction("[+] 创建新项目");
    auto* openProjectBtn = buildQuickAction("[P] 打开项目");

    layout->addWidget(newProjectBtn);
    layout->addWidget(openProjectBtn);
    layout->addStretch(1);

    connect(newProjectBtn, &QPushButton::clicked, this, [this] {
        NewProjectDialog dialog(this);
        dialog.exec();
    });

    connect(openProjectBtn, &QPushButton::clicked, this, [] {
        // Reserved for open project action.
    });

    return wrapper;
}

