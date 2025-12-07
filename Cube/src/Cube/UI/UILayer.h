#pragma once

#include "UIWidget.h"

namespace Cube {

    class UILayer{
    public:
        UILayer() = default;
        ~UILayer() = default;

        void onUpdate(float deltaTime);

        void addWidget(UIWidget* widget) { widgets.emplace_back(widget); }
    private:
        std::vector<std::unique_ptr<UIWidget>> widgets;
    };

}