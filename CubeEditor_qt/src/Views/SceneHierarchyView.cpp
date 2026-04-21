#include "SceneHierarchyView.h"

#include <QLabel>
#include <QVBoxLayout>

SceneHierarchyView::SceneHierarchyView(QWidget* parent)
    : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    auto* label = new QLabel("Scene Hierarchy View", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
