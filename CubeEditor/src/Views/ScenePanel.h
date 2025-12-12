#pragma once

#include "View.h"

class ScenePanel : public View {
public:
    ScenePanel() = default;
    ~ScenePanel() override = default;

    void render(float deltaTime) override;

private:
};