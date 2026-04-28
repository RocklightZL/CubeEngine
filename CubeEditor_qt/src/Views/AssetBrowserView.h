#pragma once

#include <QWidget>

class QTreeWidget;

class AssetBrowserView final : public QWidget {
public:
    explicit AssetBrowserView(QWidget* parent = nullptr);

private:
    void rebuildTree();

    QTreeWidget* m_tree = nullptr;
};
