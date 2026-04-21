#pragma once

#include <QDialog>

class QLineEdit;

class NewProjectDialog final : public QDialog {
public:
    explicit NewProjectDialog(QWidget* parent = nullptr);

    QString projectName() const;
    QString projectDirectory() const;

private:
    QLineEdit* m_nameEdit = nullptr;
    QLineEdit* m_pathEdit = nullptr;
};
