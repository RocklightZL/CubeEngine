#pragma once

#include <QWidget>

#include <QString>

class QTreeWidget;
class QPoint;

class AssetBrowserView final : public QWidget {
public:
    explicit AssetBrowserView(const QString& projectFilePath, QWidget* parent = nullptr);

private:
    void rebuildTree();
    void onCustomContextMenuRequested(const QPoint& pos);
    void importAssetFromFile();

    QTreeWidget* m_tree = nullptr;
};
