#include "AssetPropertyView.h"

#include <QLabel>
#include <QVBoxLayout>

AssetPropertyView::AssetPropertyView(QWidget* parent)
    : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    auto* label = new QLabel("Asset Property View", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
