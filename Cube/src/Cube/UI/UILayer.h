#pragma once

#include "UIWidget.h"
#include "Cube/Core/Layer.h"

namespace Cube {

    class UILayer : public Layer{
    public:
        UILayer() = default;
        ~UILayer() override = default;

        void onUpdate(float deltaTime) override;

        void addWidget(const std::shared_ptr<UIWidget>& widget) { widgets.push_back(widget); }
    private:
        std::vector<std::shared_ptr<UIWidget>> widgets;
    };

}