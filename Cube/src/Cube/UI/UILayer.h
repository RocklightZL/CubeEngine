#pragma once

#include "UIWidget.h"
#include "Cube/Core/Layer.h"

namespace Cube {

    class UILayer : public Layer{
    public:
        UILayer() = default;
        ~UILayer() override = default;

        void onUpdate(float deltaTime) override;

        void addWidget(UIWidget* widget) { widgets.emplace_back(widget); }
    private:
        std::vector<std::unique_ptr<UIWidget>> widgets;
    };

}