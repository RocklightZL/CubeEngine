#include "AssetBrowserView.h"

#include <QLabel>
#include <QVBoxLayout>

AssetBrowserView::AssetBrowserView(QWidget* parent)
    : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    auto* label = new QLabel("Asset Browser View", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
