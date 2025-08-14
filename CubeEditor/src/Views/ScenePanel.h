#pragma once

#include "View.h"

namespace Cube {

    class ScenePanel : public View {
    public:
        ScenePanel() = default;
        ~ScenePanel() override = default;

        void render(float deltaTime) override;
    };

}