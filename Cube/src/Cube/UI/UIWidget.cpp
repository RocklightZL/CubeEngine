#include "pch.h"
#include "UIWidget.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Core/Log.h"

namespace Cube {

    // UIWidget
    UIStyle UIWidget::globalStyle = {
        {0.0f, 0.0f, 0.0f, 1.0f},  // bgColor
        {0.0f, 0.0f, 0.0f, 0.0f},  // border: top right bottom left
        {0.0f, 0.0f, 0.0f, 1.0f}   // borderColor
    };

    void UIWidget::render() {
        if(!visible) return;
        glm::vec2 renderPos = getWorldPosition();
        Renderer2D::drawQuad(renderPos + size / 2.0f, size, nullptr, style.bgColor);

        Renderer2D::drawLine(renderPos + glm::vec2(0, size.y), renderPos + size, style.borderColor, style.border.x);
        Renderer2D::drawLine(renderPos + size, renderPos + glm::vec2(size.x, 0), style.borderColor, style.border.y);
        Renderer2D::drawLine(renderPos + glm::vec2(size.x, 0), renderPos, style.borderColor, style.border.z);
        Renderer2D::drawLine(renderPos, renderPos + glm::vec2(0, size.y), style.borderColor, style.border.w);
    }

    glm::vec2 UIWidget::getWorldPosition() const {
        if (parent) {
            return parent->getWorldPosition() + pos;
        }
        return pos;
    }

    // UIPanel
    void UIPanel::render() {
        if(!isVisible()) return;
        UIWidget::render();
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