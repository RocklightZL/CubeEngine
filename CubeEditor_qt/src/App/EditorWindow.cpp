#include "EditorWindow.h"

#include <QLabel>

EditorWindow::EditorWindow(const QString& projectFilePath, QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("CubeEditor - " + projectFilePath);
    resize(1280, 760);

    auto* placeholder = new QLabel("Editor Placeholder", this);
    placeholder->setAlignment(Qt::AlignCenter);
    setCentralWidget(placeholder);
}
