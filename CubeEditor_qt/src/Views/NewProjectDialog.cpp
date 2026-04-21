#include "NewProjectDialog.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

NewProjectDialog::NewProjectDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("创建新项目");
    setModal(true);
    resize(520, 210);

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(16, 14, 16, 14);
    root->setSpacing(8);

    auto* nameLabel = new QLabel("项目名称", this);
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("输入项目名称...");

    auto* pathLabel = new QLabel("项目路径", this);
    auto* pathRow = new QHBoxLayout();
    pathRow->setSpacing(6);
    m_pathEdit = new QLineEdit(this);
    m_pathEdit->setPlaceholderText("选择项目目录...");
    auto* browseButton = new QPushButton("浏览", this);
    browseButton->setFixedWidth(76);

    pathRow->addWidget(m_pathEdit, 1);
    pathRow->addWidget(browseButton);

    auto* actionRow = new QHBoxLayout();
    actionRow->addStretch(1);
    auto* cancelButton = new QPushButton("取消", this);
    auto* createButton = new QPushButton("创建", this);
    createButton->setProperty("role", "primary");
    actionRow->addWidget(cancelButton);
    actionRow->addWidget(createButton);

    root->addWidget(nameLabel);
    root->addWidget(m_nameEdit);
    root->addWidget(pathLabel);
    root->addLayout(pathRow);
    root->addStretch(1);
    root->addLayout(actionRow);

    setStyleSheet(R"(
        QDialog {
            background: #252526;
            color: #f3f3f3;
        }
        QLabel {
            color: #f3f3f3;
            font-size: 13px;
        }
        QLineEdit {
            min-height: 28px;
            padding: 0 8px;
            border: 1px solid #3f3f46;
            border-radius: 3px;
            background: #1f1f1f;
            color: #f3f3f3;
        }
        QPushButton {
            min-height: 28px;
            min-width: 76px;
            padding: 0 10px;
            border: 1px solid #4a4a52;
            border-radius: 3px;
            background: #2d2d30;
            color: #f3f3f3;
        }
        QPushButton:hover {
            background: #37373c;
        }
        QPushButton:pressed {
            background: #202024;
        }
        QPushButton[role="primary"] {
            background: #0e639c;
            border: 1px solid #1177bb;
        }
        QPushButton[role="primary"]:hover {
            background: #1177bb;
        }
    )");

    connect(browseButton, &QPushButton::clicked, this, [this] {
        const QString dir = QFileDialog::getExistingDirectory(this, "选择项目目录", m_pathEdit->text());
        if(!dir.isEmpty()) {
            m_pathEdit->setText(dir);
        }
    });
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(createButton, &QPushButton::clicked, this, &QDialog::accept);
}

QString NewProjectDialog::projectName() const {
    return m_nameEdit->text();
}

QString NewProjectDialog::projectDirectory() const {
    return m_pathEdit->text();
}
