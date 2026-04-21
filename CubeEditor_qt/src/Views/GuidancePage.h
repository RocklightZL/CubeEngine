#pragma once

#include <QWidget>

class GuidancePage final : public QWidget {
public:
    explicit GuidancePage(QWidget* parent = nullptr);

private:
    QWidget* buildLeftPanel();
    QWidget* buildRightPanel();
};
