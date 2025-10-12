#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <functional>

namespace Cube {

    // 前向声明
    class UICanvas;
    class Event;

    /**
     * UI基础组件类 - 所有UI元素的基类
     */
    class UIWidget {
    public:
        UIWidget() = default;
        UIWidget(const glm::vec2& pos, const glm::vec2& size) : position(pos), size(size){};

        virtual ~UIWidget() = default;

        // 基本属性
        void setPosition(const glm::vec2& pos) { position = pos; markDirty(); }
        void setSize(const glm::vec2& size) { this->size = size; markDirty(); }
        void setVisible(bool visible) { this->visible = visible; }
        void setEnabled(bool enabled) { this->enabled = enabled; }

        glm::vec2 getPosition() const { return position; }
        glm::vec2 getSize() const { return size; }
        glm::vec2 getWorldPosition() const;
        bool isVisible() const { return visible; }
        bool isEnabled() const { return enabled; }

        // 渲染和更新
        virtual void update(float deltaTime);
        virtual void render();
        
        // 事件处理
        virtual bool onMouseEnter() { return false; }
        virtual bool onMouseLeave() { return false; }
        virtual bool onMousePressed(const glm::vec2& mousePos) { return false; }
        virtual bool onMouseReleased(const glm::vec2& mousePos) { return false; }
        virtual bool onMouseMoved(const glm::vec2& mousePos) { return false; }
        virtual bool onKeyPressed(int keyCode) { return false; }
        virtual bool onKeyReleased(int keyCode) { return false; }

        // 碰撞检测
        bool containsPoint(const glm::vec2& point) const;
        
        // 样式
        struct Style {
            glm::vec4 backgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            glm::vec4 borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            float borderWidth = 0.0f;
            glm::vec4 padding = glm::vec4(0.0f); // left, top, right, bottom
            glm::vec4 margin = glm::vec4(0.0f);  // left, top, right, bottom
        };

        Style& getStyle() { return style; }
        const Style& getStyle() const { return style; }

    protected:
        // 基本属性
        glm::vec2 position = glm::vec2(0.0f);
        glm::vec2 size = glm::vec2(100.0f);
        bool visible = true;
        bool enabled = true;
        bool dirty = true;  // 是否需要重新计算

        // 层次结构
        UIWidget* parent = nullptr;

        // 样式
        Style style;

        // 内部方法
        void markDirty() { dirty = true; }
    };

    /**
     * UI按钮组件
     */
    class UIButton : public UIWidget {
    public:
        UIButton(const std::string& text = "Button");
        
        void setText(const std::string& text) { this->text = text; markDirty(); }
        const std::string& getText() const { return text; }
        
        void setOnClick(std::function<void()> callback) { onClick = callback; }
        
        void render() override;
        bool onMousePressed(const glm::vec2& mousePos) override;
        bool onMouseEnter() override;
        bool onMouseLeave() override;

    private:
        std::string text;
        std::function<void()> onClick;
        bool isPressed = false;
        bool isHovered = false;
    };

    /**
     * UI文本标签组件
     */
    class UILabel : public UIWidget {
    public:
        UILabel(const std::string& text = "Label");
        
        void setText(const std::string& text) { this->text = text; markDirty(); }
        const std::string& getText() const { return text; }
        
        void setTextColor(const glm::vec4& color) { textColor = color; }
        const glm::vec4& getTextColor() const { return textColor; }
        
        void render() override;

    private:
        std::string text;
        glm::vec4 textColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    /**
     * UI面板组件 - 容器类型
     */
    class UIPanel : public UIWidget {
    public:
        UIPanel();
        
        void render() override;
        
    protected:
        void updateLayout();
    };

}