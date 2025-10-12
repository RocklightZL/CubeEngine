#pragma once

#include "UIWidget.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace Cube {

    /**
     * UI画布 - UI元素的容器，管理一组相关的UI组件，相当于一个个的模块
     * 支持不同的渲染模式和层级管理
     */
    class UICanvas {
    public:
        enum class RenderMode {
            ScreenSpaceOverlay,    // 屏幕空间覆盖（2D UI）// TODO: 目前只支持这种模式，这是覆盖于屏幕上的UI，用UI层来管理。
            ScreenSpaceCamera,     // 屏幕空间相机
            WorldSpace             // 世界空间（3D UI）
        };

        UICanvas(RenderMode mode = RenderMode::ScreenSpaceOverlay);
        virtual ~UICanvas() = default;

        // UI元素管理
        void addWidget(const std::shared_ptr<UIWidget>& widget);
        void removeWidget(const std::shared_ptr<UIWidget>& widget);
        void clearWidgets();

        // 更新和渲染
        virtual void update(float deltaTime);
        virtual void render();

    protected:
        RenderMode renderMode;
        bool visible = true;
        int sortOrder = 0;  // 渲染顺序，数值越大越靠前
        
        glm::vec2 canvasSize = glm::vec2(1920.0f, 1080.0f);
        float scale = 1.0f;
        
        std::vector<std::shared_ptr<UIWidget>> widgets;
    };

    /**
     * 游戏HUD画布 - 专门用于游戏内HUD元素
     */
    class HUDCanvas : public UICanvas {
    public:
        HUDCanvas();
        
        // HUD特有的功能
        void setHealthBar(float health, float maxHealth);
        void setScore(int score);
        void setAmmo(int current, int total);
        
    private:
        std::shared_ptr<UILabel> healthLabel;
        std::shared_ptr<UIPanel> healthBar;
        std::shared_ptr<UILabel> scoreLabel;
        std::shared_ptr<UILabel> ammoLabel;
        
        void setupHUDElements();
    };

    /**
     * 菜单画布 - 专门用于游戏菜单
     */
    class MenuCanvas : public UICanvas {
    public:
        MenuCanvas();
        
        // 菜单特有的功能
        void showMainMenu();
        void showPauseMenu();
        void showSettingsMenu();
        void hideMenu();
        
        // 菜单回调设置
        void setOnNewGame(std::function<void()> callback) { onNewGame = callback; }
        void setOnLoadGame(std::function<void()> callback) { onLoadGame = callback; }
        void setOnSettings(std::function<void()> callback) { onSettings = callback; }
        void setOnQuit(std::function<void()> callback) { onQuit = callback; }
        
    private:
        enum class MenuState {
            Hidden,
            MainMenu,
            PauseMenu,
            SettingsMenu
        };
        
        MenuState currentState = MenuState::Hidden;
        
        // 菜单回调
        std::function<void()> onNewGame;
        std::function<void()> onLoadGame;
        std::function<void()> onSettings;
        std::function<void()> onQuit;
        
        // 菜单元素
        std::shared_ptr<UIPanel> mainMenuPanel;
        std::shared_ptr<UIPanel> pauseMenuPanel;
        std::shared_ptr<UIPanel> settingsMenuPanel;
        
        void setupMainMenu();
        void setupPauseMenu();
        void setupSettingsMenu();
    };

}