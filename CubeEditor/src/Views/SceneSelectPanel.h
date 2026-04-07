#pragma once
#include "View.h"

class SceneSelectPanel : public View {
public:
    SceneSelectPanel(EditorPage& editorPage) : View(editorPage) {}
    ~SceneSelectPanel() override = default;
    
    void render(float deltaTime) override;
};