#include "NewProjectDialog.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

NewProjectDialog::NewProjectDialog(QWidget* parent)
    : QDialog(parent) {
    setObjectName("newProjectDialog");
    setWindowTitle("Create New Project");
    setModal(true);
    resize(520, 210);

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(16, 14, 16, 14);
    root->setSpacing(8);

    auto* nameLabel = new QLabel("Project Name", this);
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("Enter project name...");

    auto* pathLabel = new QLabel("Project Location", this);
    auto* pathRow = new QHBoxLayout();
    pathRow->setSpacing(6);
    m_pathEdit = new QLineEdit(this);
    m_pathEdit->setPlaceholderText("Select project location...");
    auto* browseButton = new QPushButton("Browse", this);
    browseButton->setFixedWidth(76);

    pathRow->addWidget(m_pathEdit, 1);
    pathRow->addWidget(browseButton);

    m_locationHintLabel = new QLabel(this);
    m_locationHintLabel->setObjectName("locationHint");
    m_locationHintLabel->setWordWrap(true);

    auto* actionRow = new QHBoxLayout();
    actionRow->addStretch(1);
    auto* cancelButton = new QPushButton("Cancel", this);
    auto* createButton = new QPushButton("Create", this);
    createButton->setProperty("role", "primary");
    actionRow->addWidget(cancelButton);
    actionRow->addWidget(createButton);

    root->addWidget(nameLabel);
    root->addWidget(m_nameEdit);
    root->addWidget(pathLabel);
    root->addLayout(pathRow);
    root->addWidget(m_locationHintLabel);
    root->addStretch(1);
    root->addLayout(actionRow);

    connect(browseButton, &QPushButton::clicked, this, [this] {
        const QString dir = QFileDialog::getExistingDirectory(this, "Select Project Location", m_pathEdit->text());
        if(!dir.isEmpty()) {
            m_pathEdit->setText(dir);
            updateLocationHint();
        }
    });
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(createButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_nameEdit, &QLineEdit::textChanged, this, [this] { updateLocationHint(); });
    connect(m_pathEdit, &QLineEdit::textChanged, this, [this] { updateLocationHint(); });

    updateLocationHint();
}

QString NewProjectDialog::projectName() const {
    return m_nameEdit->text();
}

QString NewProjectDialog::projectDirectory() const {
    return m_pathEdit->text();
}

void NewProjectDialog::updateLocationHint() {
    const QString name = m_nameEdit->text().trimmed();
    const QString location = m_pathEdit->text().trimmed();

    if(location.isEmpty()) {
        m_locationHintLabel->setText("A project subdirectory will be created in the selected location.");
        return;
    }

    const QString folderName = name.isEmpty() ? QStringLiteral("<ProjectName>") : name;
    m_locationHintLabel->setText("A subdirectory \"" + folderName + "/\" will be created under: " + location);
}
