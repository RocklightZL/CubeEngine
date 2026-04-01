#pragma once
#include "View.h"

class AssetInspector : public View {
public:
    AssetInspector(EditorPage& editorPage) : View(editorPage) {}
    ~AssetInspector() override = default;

    void render(float deltaTime) override;
};
