#pragma once
#include "View.h"

namespace Cube {

    class ToolBar : public View {
    public:
        ToolBar() = default;
        ~ToolBar() override = default;

        void render(float deltaTime) override;
    };

}