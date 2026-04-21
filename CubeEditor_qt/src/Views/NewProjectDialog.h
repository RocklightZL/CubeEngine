#pragma once

#include <QDialog>

class QLabel;
class QLineEdit;

class NewProjectDialog final : public QDialog {
public:
    explicit NewProjectDialog(QWidget* parent = nullptr);

    QString projectName() const;
    QString projectDirectory() const;

private:
    void updateLocationHint();

private:
    QLabel* m_locationHintLabel = nullptr;
    QLineEdit* m_nameEdit = nullptr;
    QLineEdit* m_pathEdit = nullptr;
};
