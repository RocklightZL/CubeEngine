#include "pch.h"
#include "UIManager.h"
#include "Cube/Event/MouseEvent.h"
#include "Cube/Event/KeyEvent.h"
#include "Cube/Core/Log.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace Cube {

    UIManager* UIManager::instance = nullptr;

    UIManager::UIManager() 
        : Layer("UIManager"), screenSize(1920.0f, 1080.0f), mousePosition(0.0f), mousePressed(false) {
        instance = this;
        updateProjectionMatrix();
    }

    UIManager::~UIManager() {
        if (instance == this) {
            instance = nullptr;
        }
    }

    void UIManager::onAttach() {
        CUBE_LOG_INFO("UIManager attached");
        
        // 初始化UI渲染系统
        Renderer2D::init();
    }

    void UIManager::onDetach() {
        CUBE_LOG_INFO("UIManager detached");
        canvases.clear();
    }

    void UIManager::onUpdate(float deltaTime) {
        processInput();
        
        // 更新所有画布
        for (auto& canvas : canvases) {
            if (canvas && canvas->isVisible()) {
                canvas->update(deltaTime);
            }
        }
    }

    void UIManager::onEvent() {
        // 事件处理在具体的事件回调中进行
    }

    void UIManager::render() {
        // 按照sortOrder排序画布
        std::sort(canvases.begin(), canvases.end(), 
            [](const std::shared_ptr<UICanvas>& a, const std::shared_ptr<UICanvas>& b) {
                return a->getSortOrder() < b->getSortOrder();
            });

        // 设置UI投影矩阵
        Renderer2D::beginFrame(uiProjectionMatrix);

        // 渲染所有可见的画布
        for (auto& canvas : canvases) {
            if (canvas && canvas->isVisible()) {
                canvas->render();
            }
        }

        Renderer2D::endFrame();
    }

    void UIManager::addCanvas(std::shared_ptr<UICanvas> canvas) {
        if (canvas) {
            canvases.push_back(canvas);
        }
    }

    void UIManager::removeCanvas(std::shared_ptr<UICanvas> canvas) {
        auto it = std::find(canvases.begin(), canvases.end(), canvas);
        if (it != canvases.end()) {
            canvases.erase(it);
        }
    }

    void UIManager::setScreenSize(int width, int height) {
        screenSize = glm::vec2(width, height);
        updateProjectionMatrix();
        
        // 通知所有画布屏幕尺寸变化
        for (auto& canvas : canvases) {
            if (canvas) {
                canvas->setCanvasSize(screenSize);
            }
        }
    }

    bool UIManager::onMouseMoved(const Event& e) {
        if (e.getType() == EventType::MouseMoved) {
            const MouseMovedEvent& mouseEvent = static_cast<const MouseMovedEvent&>(e);
            mousePosition = glm::vec2(mouseEvent.getX(), screenSize.y - mouseEvent.getY()); // 翻转Y轴
            
            // 将事件传递给画布处理（从前往后）
            for (auto it = canvases.rbegin(); it != canvases.rend(); ++it) {
                if ((*it) && (*it)->isVisible()) {
                    if ((*it)->handleMouseMoved(mousePosition)) {
                        return true; // 事件被消费
                    }
                }
            }
        }
        return false;
    }

    bool UIManager::onMousePressed(const Event& e) {
        if (e.getType() == EventType::MousePressed) {
            const MousePressedEvent& mouseEvent = static_cast<const MousePressedEvent&>(e);
            mousePosition = glm::vec2(mouseEvent.getX(), screenSize.y - mouseEvent.getY()); // 翻转Y轴
            mousePressed = true;
            
            // 将事件传递给画布处理（从前往后）
            for (auto it = canvases.rbegin(); it != canvases.rend(); ++it) {
                if ((*it) && (*it)->isVisible()) {
                    if ((*it)->handleMousePressed(mousePosition)) {
                        return true; // 事件被消费
                    }
                }
            }
        }
        return false;
    }

    bool UIManager::onMouseReleased(const Event& e) {
        if (e.getType() == EventType::MouseReleased) {
            const MouseReleasedEvent& mouseEvent = static_cast<const MouseReleasedEvent&>(e);
            mousePosition = glm::vec2(mouseEvent.getX(), screenSize.y - mouseEvent.getY()); // 翻转Y轴
            mousePressed = false;
            
            // 将事件传递给画布处理（从前往后）
            for (auto it = canvases.rbegin(); it != canvases.rend(); ++it) {
                if ((*it) && (*it)->isVisible()) {
                    if ((*it)->handleMouseReleased(mousePosition)) {
                        return true; // 事件被消费
                    }
                }
            }
        }
        return false;
    }

    bool UIManager::onKeyPressed(const Event& e) {
        if (e.getType() == EventType::KeyPressed) {
            const KeyPressedEvent& keyEvent = static_cast<const KeyPressedEvent&>(e);
            int keyCode = static_cast<int>(keyEvent.getKeyCode());
            
            // 将事件传递给画布处理
            for (auto it = canvases.rbegin(); it != canvases.rend(); ++it) {
                if ((*it) && (*it)->isVisible()) {
                    if ((*it)->handleKeyPressed(keyCode)) {
                        return true; // 事件被消费
                    }
                }
            }
        }
        return false;
    }

    bool UIManager::onKeyReleased(const Event& e) {
        if (e.getType() == EventType::KeyReleased) {
            const KeyReleasedEvent& keyEvent = static_cast<const KeyReleasedEvent&>(e);
            int keyCode = static_cast<int>(keyEvent.getKeyCode());
            
            // 将事件传递给画布处理
            for (auto it = canvases.rbegin(); it != canvases.rend(); ++it) {
                if ((*it) && (*it)->isVisible()) {
                    if ((*it)->handleKeyReleased(keyCode)) {
                        return true; // 事件被消费
                    }
                }
            }
        }
        return false;
    }

    UIManager& UIManager::getInstance() {
        if (!instance) {
            // 如果没有实例，创建一个默认实例
            static UIManager defaultInstance;
            return defaultInstance;
        }
        return *instance;
    }

    void UIManager::updateProjectionMatrix() {
        // 创建正交投影矩阵，用于2D UI渲染
        // 左下角为(0,0)，右上角为(screenSize.x, screenSize.y)
        uiProjectionMatrix = glm::ortho(0.0f, screenSize.x, 0.0f, screenSize.y, -1.0f, 1.0f);
    }

    void UIManager::processInput() {
        // 这里可以处理一些全局的UI输入逻辑
        // 比如快捷键、全局手势等
    }

}