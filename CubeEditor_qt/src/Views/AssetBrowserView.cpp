#include "AssetBrowserView.h"

#include "../App/AssetSystem.h"

#include <functional>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QTreeWidget>
#include <QVBoxLayout>

AssetBrowserView::AssetBrowserView(const QString& projectFilePath, QWidget* parent)
    : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    m_tree = new QTreeWidget(this);
    m_tree->setIndentation(15);
    m_tree->setHeaderHidden(true);
    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    layout->addWidget(m_tree);

    connect(m_tree, &QTreeWidget::customContextMenuRequested, this, &AssetBrowserView::onCustomContextMenuRequested);

    auto& assets = CubeEditor::AssetSystem::get();
    if(!assets.loadFromProject(projectFilePath)) {
        QMessageBox::warning(this, "Asset Browser", "Failed to load AssetMap.json.");
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

void AssetBrowserView::onCustomContextMenuRequested(const QPoint& pos) {
    QMenu menu(this);
    QAction* importAction = menu.addAction("Import Asset...");

    QAction* picked = menu.exec(m_tree->viewport()->mapToGlobal(pos));
    if(picked == importAction) {
        importAssetFromFile();
    }
}

void AssetBrowserView::importAssetFromFile() {
    auto& assets = CubeEditor::AssetSystem::get();
    const QString assetsDir = assets.getAssetsDirPath();
    const QString filePath = QFileDialog::getOpenFileName(this, "Import Asset", assetsDir);
    if(filePath.isEmpty()) {
        return;
    }

    QString error;
    if(!assets.importFile(filePath, &error)) {
        const QString message = error.isEmpty() ? "Import failed." : error;
        QMessageBox::warning(this, "Import Asset", message);
        return;
    }

    rebuildTree();
}
