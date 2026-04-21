#include "AnimationEditorView.h"

#include <QLabel>
#include <QVBoxLayout>

AnimationEditorView::AnimationEditorView(QWidget* parent)
    : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    auto* label = new QLabel("Animation Editor View", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
