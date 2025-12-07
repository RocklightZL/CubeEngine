#pragma once
#include "View.h"

class SceneSelectPanel : public View {
public:
    SceneSelectPanel() = default;
    ~SceneSelectPanel() override = default;

    void render(float deltaTime) override;
};