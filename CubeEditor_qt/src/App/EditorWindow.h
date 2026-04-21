#pragma once

#include <QMainWindow>

class EditorWindow final : public QMainWindow {
public:
    explicit EditorWindow(const QString& projectFilePath, QWidget* parent = nullptr);
};
