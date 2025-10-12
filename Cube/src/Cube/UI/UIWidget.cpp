#include "pch.h"
#include "UIWidget.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Core/Log.h"

#include <algorithm>

namespace Cube {

    // =================================
    // UIWidget Implementation
    // =================================

    glm::vec2 UIWidget::getWorldPosition() const {
        glm::vec2 worldPos = position;
        if (parent) {
            worldPos += parent->getWorldPosition();
        }
        return worldPos;
    }

    void UIWidget::addChild(std::shared_ptr<UIWidget> child) {
        if (child && std::find(children.begin(), children.end(), child) == children.end()) {
            children.push_back(child);
            child->setParent(this);
            markDirty();
        }
    }

    void UIWidget::removeChild(std::shared_ptr<UIWidget> child) {
        auto it = std::find(children.begin(), children.end(), child);
        if (it != children.end()) {
            (*it)->setParent(nullptr);
            children.erase(it);
            markDirty();
        }
    }

    void UIWidget::update(float deltaTime) {
        if (!visible) return;

        if (dirty) {
            updateLayout();
            dirty = false;
        }

        updateChildren(deltaTime);
    }

    void UIWidget::render() {
        if (!visible) return;

        // 渲染背景
        if (style.backgroundColor.a > 0.0f) {
            glm::vec2 worldPos = getWorldPosition();
            Renderer2D::drawQuad(worldPos, size, nullptr, style.backgroundColor);
        }

        // 渲染边框
        if (style.borderWidth > 0.0f && style.borderColor.a > 0.0f) {
            glm::vec2 worldPos = getWorldPosition();
            
            // 绘制四条边框线（简化实现）
            // 这里可以进一步优化为专门的边框渲染
            float borderWidth = style.borderWidth;
            
            // 上边框
            Renderer2D::drawQuad(glm::vec2(worldPos.x, worldPos.y + size.y - borderWidth), 
                               glm::vec2(size.x, borderWidth), nullptr, style.borderColor);
            // 下边框
            Renderer2D::drawQuad(worldPos, 
                               glm::vec2(size.x, borderWidth), nullptr, style.borderColor);
            // 左边框
            Renderer2D::drawQuad(worldPos, 
                               glm::vec2(borderWidth, size.y), nullptr, style.borderColor);
            // 右边框
            Renderer2D::drawQuad(glm::vec2(worldPos.x + size.x - borderWidth, worldPos.y), 
                               glm::vec2(borderWidth, size.y), nullptr, style.borderColor);
        }

        renderChildren();
    }

    bool UIWidget::containsPoint(const glm::vec2& point) const {
        glm::vec2 worldPos = getWorldPosition();
        return point.x >= worldPos.x && point.x <= worldPos.x + size.x &&
               point.y >= worldPos.y && point.y <= worldPos.y + size.y;
    }

    void UIWidget::updateChildren(float deltaTime) {
        for (auto& child : children) {
            if (child) {
                child->update(deltaTime);
            }
        }
    }

    void UIWidget::renderChildren() {
        for (auto& child : children) {
            if (child) {
                child->render();
            }
        }
    }

    // =================================
    // UIButton Implementation
    // =================================

    UIButton::UIButton(const std::string& text) : text(text) {
        // 设置按钮默认样式
        style.backgroundColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        style.borderColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        style.borderWidth = 2.0f;
        style.padding = glm::vec4(10.0f, 5.0f, 10.0f, 5.0f);
    }

    void UIButton::render() {
        // 根据状态调整颜色
        glm::vec4 currentColor = style.backgroundColor;
        if (isPressed) {
            currentColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        } else if (isHovered) {
            currentColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        }

        // 临时修改背景色
        glm::vec4 originalColor = style.backgroundColor;
        style.backgroundColor = currentColor;
        
        // 调用基类渲染
        UIWidget::render();
        
        // 恢复原始颜色
        style.backgroundColor = originalColor;

        // 渲染按钮文本（简化实现，实际需要文本渲染系统）
        // 这里只是占位符，后续可以集成文本渲染
        CUBE_LOG_TRACE("Rendering button text: {}", text);
    }

    bool UIButton::onMousePressed(const glm::vec2& mousePos) {
        if (enabled && containsPoint(mousePos)) {
            isPressed = true;
            if (onClick) {
                onClick();
            }
            return true;
        }
        return false;
    }

    bool UIButton::onMouseEnter() {
        isHovered = true;
        return false;
    }

    bool UIButton::onMouseLeave() {
        isHovered = false;
        isPressed = false;
        return false;
    }

    // =================================
    // UILabel Implementation
    // =================================

    UILabel::UILabel(const std::string& text) : text(text) {
        // 设置标签默认样式
        style.backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // 透明背景
    }

    void UILabel::render() {
        // 调用基类渲染
        UIWidget::render();

        // 渲染文本（简化实现）
        CUBE_LOG_TRACE("Rendering label text: {}", text);
    }

    // =================================
    // UIPanel Implementation
    // =================================

    UIPanel::UIPanel() {
        // 设置面板默认样式
        style.backgroundColor = glm::vec4(0.9f, 0.9f, 0.9f, 0.8f);
        style.borderColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        style.borderWidth = 1.0f;
    }

    void UIPanel::render() {
        // 调用基类渲染
        UIWidget::render();
    }

    void UIPanel::updateLayout() {
        // 这里可以实现自动布局逻辑
        // 比如垂直布局、水平布局等
        
        // 简单的垂直布局示例
        float currentY = position.y + style.padding.y; // top padding
        for (auto& child : children) {
            if (child && child->isVisible()) {
                child->setPosition(glm::vec2(position.x + style.padding.x, currentY));
                currentY += child->getSize().y + 5.0f; // 5px间距
            }
        }
    }

}