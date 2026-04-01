#pragma once

#include "View.h"

class ScenePanel : public View {
public:
    ScenePanel(EditorPage& editorPage) : View(editorPage) {}
    ~ScenePanel() override = default;

    void render(float deltaTime) override;

private:
};