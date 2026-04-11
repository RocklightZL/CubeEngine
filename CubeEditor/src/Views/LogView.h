#pragma once

#include "View.h"

class LogView : public View{
public:
    LogView(EditorPage& editorPage) : View(editorPage) {}
    ~LogView() override = default;

    void render(float deltaTime) override;
};