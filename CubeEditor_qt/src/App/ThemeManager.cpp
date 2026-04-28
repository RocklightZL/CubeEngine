#include "ThemeManager.h"

#include <QApplication>

namespace {
const char* kGlobalThemeQss = R"(
    QWidget {
        color: #f3f3f3;
        font-size: 13px;
    }

    QMainWindow,
    QDialog,
    QWidget#guidancePage {
        background: #252526;
    }

    QMenuBar {
        background: #252526;
        color: #f3f3f3;
        border-bottom: 1px solid #3a3a3d;
    }
    QMenuBar::item {
        padding: 4px 10px;
        background: transparent;
    }
    QMenuBar::item:selected {
        background: #313136;
    }
    QMenu {
        background: #252526;
        color: #f3f3f3;
        border: 1px solid #3a3a3d;
    }
    QMenu::item:selected {
        background: #313136;
    }

    QSplitter::handle {
        background: #2f2f31;
    }
    QSplitter::handle:hover {
        background: #3a3a40;
    }

    QTabWidget::pane {
        border: 1px solid #3a3a3d;
        background: #252526;
    }
    QTabBar::tab {
        background: #2d2d30;
        color: #d6d6d6;
        border: 1px solid #3f3f46;
        border-bottom: none;
        border-top-left-radius: 3px;
        border-top-right-radius: 3px;
        padding: 4px 10px;
        margin-right: 2px;
        min-height: 22px;
    }
    QTabBar::tab:selected {
        background: #252526;
        color: #ffffff;
        border-color: #5a5a62;
    }
    QTabBar::tab:hover {
        background: #3a3a40;
    }

    QFrame#recentPanel {
        border: 1px solid #3a3a3d;
        border-radius: 0px;
        background: #252526;
    }
    QListWidget#recentList {
        border: none;
        background: transparent;
        outline: none;
    }
    QListWidget#recentList::item {
        height: 30px;
        padding-left: 8px;
    }
    QListWidget#recentList::item:hover {
        background: #313136;
    }
    QListWidget#recentList::item:selected {
        background: #094771;
        color: #ffffff;
    }

    QPushButton {
        min-height: 28px;
        min-width: 76px;
        padding: 0 10px;
        border: 1px solid #4a4a52;
        border-radius: 3px;
        background: #2d2d30;
        color: #f3f3f3;
    }
    QPushButton:hover {
        background: #37373c;
    }
    QPushButton:pressed {
        background: #202024;
    }
    QPushButton#quickAction {
        text-align: left;
        min-height: 32px;
    }
    QPushButton[role="primary"] {
        background: #0e639c;
        border: 1px solid #1177bb;
    }
    QPushButton[role="primary"]:hover {
        background: #1177bb;
    }

    QLabel#locationHint {
        color: #b0b0b5;
        font-size: 12px;
    }

    QLineEdit {
        min-height: 28px;
        padding: 0 8px;
        border: 1px solid #3f3f46;
        border-radius: 3px;
        background: #1f1f1f;
        color: #f3f3f3;
    }

    QTreeWidget {
        border: none;
        background: transparent;
        outline: none;
        color: #f3f3f3;
    }
    QTreeWidget::item {
        height: 24px;
    }
    QTreeWidget::item:hover {
        background: #313136;
    }
    QTreeWidget::item:selected {
        background: #094771;
        color: #ffffff;
    }
    QTreeWidget QHeaderView::section {
        background: #2d2d30;
        color: #d6d6d6;
        border: 1px solid #3f3f46;
        padding-left: 6px;
        height: 22px;
    }

    QWidget#numericDragEdit {
        min-height: 22px;
        max-height: 22px;
        background: #252526;
        border: 1px solid #3f3f46;
        border-radius: 3px;
    }
    QWidget#numericDragEdit:hover {
        border: 1px solid #5a5a60;
    }
    QWidget#numericDragEdit QLabel {
        color: #f3f3f3;
        background: transparent;
        border: none;
    }
    QWidget#numericDragEdit QLineEdit {
        min-height: 0px;
        padding: 0 4px;
        color: #f3f3f3;
        background: transparent;
        border: none;
        border-radius: 0px;
    }
    QWidget#numericDragEdit QLineEdit:focus {
        color: #ffffff;
    }
)";
}

namespace AppTheme {
void applyGlobalTheme(QApplication& app) {
    app.setStyleSheet(QString::fromUtf8(kGlobalThemeQss));
}
}
