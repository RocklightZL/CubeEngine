#include "EntityPropertyView.h"

#include <QLabel>
#include <QVBoxLayout>

EntityPropertyView::EntityPropertyView(QWidget* parent)
    : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    auto* label = new QLabel("Entity Property View", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
