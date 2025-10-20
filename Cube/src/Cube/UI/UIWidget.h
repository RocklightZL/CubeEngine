#pragma once

#include "Cube/Renderer/Renderer.h"
#include "Cube/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Cube {
    struct UIStyle {
        glm::vec4 bgColor;
        glm::vec4 border;
        glm::vec4 borderColor;
    };

    // base class for all UI elements
    class UIWidget {
    public:
        UIWidget(){ style = globalStyle; }
        UIWidget(const glm::vec2& pos, const glm::vec2& size) : pos(pos), size(size) { style = globalStyle; }
        virtual ~UIWidget() = default;

        virtual void render();

        glm::vec2 getWorldPosition() const;

        void setParent(UIWidget* p) { parent = p; }
        UIWidget* getParent() const { return parent; }

        void setPosition(const glm::vec2& p) { pos = p; }
        glm::vec2 getPosition() const { return pos; }

        void setSize(const glm::vec2& s) { size = s; }
        glm::vec2 getSize() const { return size; }

        UIStyle& getStyle() { return style; }

        void setVisible(bool v) { visible = v; }
        bool isVisible() const { return visible; }

        static UIStyle globalStyle;

    protected:
        glm::vec2 pos = {0.0f, 0.0f};     // relative position to parent
        glm::vec2 size = {100.0f, 30.0f};
        UIStyle style;
        bool visible = true;

        UIWidget* parent = nullptr; // upper container
    };

    class UIPanel : public UIWidget {
    public:
        UIPanel() = default;
        UIPanel(std::initializer_list<std::shared_ptr<UIWidget>> children) : children(children) {}
        ~UIPanel() override = default;

        void render() override;

        void addChild(const std::shared_ptr<UIWidget>& child);
        void removeChild(const std::shared_ptr<UIWidget>& child);

    protected:
        std::vector<std::shared_ptr<UIWidget>> children;
    };

    class UIImage : public UIWidget {
    public:
        UIImage() = default;
        UIImage(const std::shared_ptr<Texture2D>& texture, const glm::vec2& pos = {0.0f, 0.0f}, const glm::vec2& size = {100.0f, 30.0f}) : UIWidget(pos, size), texture(texture) {}
        ~UIImage() override = default;

        void render() override;
    protected:
        std::shared_ptr<Texture2D> texture;
    };

    class UILabel : public UIWidget {
    public:
        UILabel() = default;
        UILabel(const std::string& text, const glm::vec2& pos = {0.0f, 0.0f}, const glm::vec2& size = {100.0f, 30.0f}) : UIWidget(pos, size), text(text) {}
        ~UILabel() override = default;

        void render() override;
    protected:
        std::string text;
    };
}