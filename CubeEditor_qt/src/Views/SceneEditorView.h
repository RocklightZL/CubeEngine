#pragma once

#include <QWidget>

class SceneEditorView final : public QWidget {
public:
    explicit SceneEditorView(const QString& sceneName, QWidget* parent = nullptr);
};
