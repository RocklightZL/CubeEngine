#pragma once

#include "Cube/Core/Layer.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Event/Event.h"
#include "UIWidget.h"
#include "UICanvas.h"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace Cube {

    /**
     * UI管理器 - 负责管理所有UI画布和全局UI状态
     * 作为一个Layer存在，独立于游戏场景
     */
    class UIManager : public Layer {
    public:
        UIManager();
        virtual ~UIManager();

        // Layer接口实现
        void onAttach() override;
        void onDetach() override;
        void onUpdate(float deltaTime) override;
        void onEvent() override;

        // UI管理接口
        void render();
        void addCanvas(std::shared_ptr<UICanvas> canvas);
        void removeCanvas(std::shared_ptr<UICanvas> canvas);
        void setScreenSize(int width, int height);

        // 事件处理
        bool onMouseMoved(const Event& e);
        bool onMousePressed(const Event& e);
        bool onMouseReleased(const Event& e);
        bool onKeyPressed(const Event& e);
        bool onKeyReleased(const Event& e);

        // 获取单例实例
        static UIManager& getInstance();

        // 输入状态
        glm::vec2 getMousePosition() const { return mousePosition; }
        bool isMousePressed() const { return mousePressed; }

    private:
        static UIManager* instance;
        
        std::vector<std::shared_ptr<UICanvas>> canvases;
        glm::vec2 screenSize;
        glm::vec2 mousePosition;
        bool mousePressed;
        
        // UI渲染状态
        glm::mat4 uiProjectionMatrix;
        
        void updateProjectionMatrix();
        void processInput();
    };

}