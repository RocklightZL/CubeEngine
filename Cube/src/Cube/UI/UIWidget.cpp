#include "pch.h"
#include "UIWidget.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Core/Log.h"

#include <algorithm>

namespace Cube {

    // UIWidget
    UIStyle UIWidget::globalStyle = {
        {0.0f, 0.0f, 0.0f, 1.0f},  // bgColor
        {0.0f, 0.0f, 0.0f, 0.0f},  // border
        {0.0f, 0.0f, 0.0f, 1.0f}   // borderColor
    };

    void UIWidget::render() {
        
    }

    // UIPanel
    void UIPanel::render() {
        if(!isVisible()) return;
        for(auto& child : children) {
            child->render();
        }
    }

    void UIPanel::addChild(const std::shared_ptr<UIWidget>& child) {
        child->setParent(this);
        children.push_back(child);
    }

    void UIPanel::removeChild(const std::shared_ptr<UIWidget>& child) { children.erase(std::remove(children.begin(), children.end(), child), children.end()); }

    // UIImage
    void UIImage::render() {
        
    }

}  // namespace Cube