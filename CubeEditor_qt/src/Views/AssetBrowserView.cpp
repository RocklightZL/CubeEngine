#include "AssetBrowserView.h"

#include "../App/AssetSystem.h"

#include <functional>
#include <QTreeWidget>
#include <QVBoxLayout>

AssetBrowserView::AssetBrowserView(QWidget* parent)
    : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    m_tree = new QTreeWidget(this);
    m_tree->setIndentation(15);
    m_tree->setHeaderHidden(true);
    layout->addWidget(m_tree);

    auto& assets = CubeEditor::AssetSystem::get();
    if(assets.getRoot()->getChildren().empty()) {
        auto* textures = assets.createGroup(nullptr, "Textures");
        assets.createResource(textures, "texture/brick");
        assets.createResource(textures, "texture/metal");

        auto* sprites = assets.createGroup(nullptr, "Sprites");
        assets.createResource(sprites, "sprite/hero");
    }

    rebuildTree();
}

void AssetBrowserView::rebuildTree() {
    m_tree->clear();

    auto& assets = CubeEditor::AssetSystem::get();
    CubeEditor::AssetGroup* root = assets.getRoot();
    if(!root) {
        return;
    }

    auto* rootItem = new QTreeWidgetItem(m_tree);
    rootItem->setText(0, QString::fromStdString(root->getName()));

    std::function<void(QTreeWidgetItem*, const CubeEditor::AssetGroup*)> addGroup;
    addGroup = [&](QTreeWidgetItem* parentItem, const CubeEditor::AssetGroup* group) {
        if(!group) {
            return;
        }
        for(const auto& child : group->getChildren()) {
            if(!child) {
                continue;
            }
            if(child->isGroup()) {
                auto* groupItem = new QTreeWidgetItem(parentItem);
                groupItem->setText(0, QString::fromStdString(child->getName()));
                addGroup(groupItem, static_cast<const CubeEditor::AssetGroup*>(child.get()));
            } else {
                auto* assetItem = static_cast<const CubeEditor::AssetItem*>(child.get());
                auto* item = new QTreeWidgetItem(parentItem);
                item->setText(0, QString::fromStdString(assetItem->getIdentifier()));
            }
        }
    };

    addGroup(rootItem, root);
    m_tree->expandAll();
    m_tree->resizeColumnToContents(0);
}
