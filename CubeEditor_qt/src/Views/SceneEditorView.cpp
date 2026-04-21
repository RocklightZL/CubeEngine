#include "SceneEditorView.h"

#include <QLabel>
#include <QVBoxLayout>

SceneEditorView::SceneEditorView(const QString& sceneName, QWidget* parent)
    : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    auto* label = new QLabel("Scene Workspace: " + sceneName, this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
