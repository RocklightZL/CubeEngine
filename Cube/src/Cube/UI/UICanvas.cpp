#include "pch.h"
#include "UICanvas.h"
#include "Cube/Core/Log.h"

#include <algorithm>

namespace Cube {

    // =================================
    // UICanvas Implementation
    // =================================

    UICanvas::UICanvas(RenderMode mode) : renderMode(mode) {
        // 构造函数
    }

    void UICanvas::addWidget(const std::shared_ptr<UIWidget>& widget) {
        if (widget && std::find(widgets.begin(), widgets.end(), widget) == widgets.end()) {
            widgets.push_back(widget);
        }
    }

    void UICanvas::removeWidget(const std::shared_ptr<UIWidget>& widget) {
        auto it = std::find(widgets.begin(), widgets.end(), widget);
        if (it != widgets.end()) {
            widgets.erase(it);
        }
    }

    void UICanvas::clearWidgets() {
        widgets.clear();
    }

    void UICanvas::update(float deltaTime) {
        if (!visible) return;

        // 更新所有UI组件
        for (auto& widget : widgets) {
            widget->update(deltaTime);
        }
    }

    void UICanvas::render() {
        if (!visible) return;

        // TODO: 根据渲染模式设置不同的渲染状态
        switch (renderMode) {
            case RenderMode::ScreenSpaceOverlay:
                // 2D屏幕空间渲染，不需要特殊设置
                break;
            case RenderMode::ScreenSpaceCamera:
                // 相机空间渲染
                break;
            case RenderMode::WorldSpace:
                // 世界空间渲染
                break;
        }

        // 渲染所有UI组件
        for (auto& widget : widgets) {
            widget->render();
        }
    }

    // =================================
    // HUDCanvas Implementation
    // =================================

    HUDCanvas::HUDCanvas() : UICanvas(RenderMode::ScreenSpaceOverlay) {
        sortOrder = 100; // HUD通常在最前面
        setupHUDElements();
    }

    void HUDCanvas::setHealthBar(float health, float maxHealth) {
        if (healthLabel) {
            std::string healthText = "Health: " + std::to_string(static_cast<int>(health)) + "/" + std::to_string(static_cast<int>(maxHealth));
            healthLabel->setText(healthText);
        }
        
        if (healthBar) {
            // 更新血条长度
            float healthRatio = maxHealth > 0 ? health / maxHealth : 0.0f;
            glm::vec2 originalSize = glm::vec2(200.0f, 20.0f);
            healthBar->setSize(glm::vec2(originalSize.x * healthRatio, originalSize.y));
            
            // 根据血量设置颜色
            if (healthRatio > 0.6f) {
                healthBar->getStyle().backgroundColor = glm::vec4(0.0f, 1.0f, 0.0f, 0.8f); // 绿色
            } else if (healthRatio > 0.3f) {
                healthBar->getStyle().backgroundColor = glm::vec4(1.0f, 1.0f, 0.0f, 0.8f); // 黄色
            } else {
                healthBar->getStyle().backgroundColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.8f); // 红色
            }
        }
    }

    void HUDCanvas::setScore(int score) {
        if (scoreLabel) {
            scoreLabel->setText("Score: " + std::to_string(score));
        }
    }

    void HUDCanvas::setAmmo(int current, int total) {
        if (ammoLabel) {
            ammoLabel->setText("Ammo: " + std::to_string(current) + "/" + std::to_string(total));
        }
    }

    void HUDCanvas::setupHUDElements() {
        // 创建血量标签
        healthLabel = std::make_shared<UILabel>("Health: 100/100");
        healthLabel->setPosition(glm::vec2(10.0f, canvasSize.y - 30.0f));
        healthLabel->setSize(glm::vec2(200.0f, 25.0f));
        addWidget(healthLabel);

        // 创建血条
        healthBar = std::make_shared<UIPanel>();
        healthBar->setPosition(glm::vec2(10.0f, canvasSize.y - 60.0f));
        healthBar->setSize(glm::vec2(200.0f, 20.0f));
        healthBar->getStyle().backgroundColor = glm::vec4(0.0f, 1.0f, 0.0f, 0.8f);
        healthBar->getStyle().borderWidth = 2.0f;
        healthBar->getStyle().borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        addWidget(healthBar);

        // 创建分数标签
        scoreLabel = std::make_shared<UILabel>("Score: 0");
        scoreLabel->setPosition(glm::vec2(canvasSize.x - 150.0f, canvasSize.y - 30.0f));
        scoreLabel->setSize(glm::vec2(140.0f, 25.0f));
        addWidget(scoreLabel);

        // 创建弹药标签
        ammoLabel = std::make_shared<UILabel>("Ammo: 30/30");
        ammoLabel->setPosition(glm::vec2(canvasSize.x - 150.0f, canvasSize.y - 60.0f));
        ammoLabel->setSize(glm::vec2(140.0f, 25.0f));
        addWidget(ammoLabel);
    }

    // =================================
    // MenuCanvas Implementation
    // =================================

    MenuCanvas::MenuCanvas() : UICanvas(RenderMode::ScreenSpaceOverlay) {
        sortOrder = 200; // 菜单在HUD之上
        setupMainMenu();
        setupPauseMenu();
        setupSettingsMenu();
        hideMenu();
    }

    void MenuCanvas::showMainMenu() {
        currentState = MenuState::MainMenu;
        if (mainMenuPanel) mainMenuPanel->setVisible(true);
        if (pauseMenuPanel) pauseMenuPanel->setVisible(false);
        if (settingsMenuPanel) settingsMenuPanel->setVisible(false);
    }

    void MenuCanvas::showPauseMenu() {
        currentState = MenuState::PauseMenu;
        if (mainMenuPanel) mainMenuPanel->setVisible(false);
        if (pauseMenuPanel) pauseMenuPanel->setVisible(true);
        if (settingsMenuPanel) settingsMenuPanel->setVisible(false);
    }

    void MenuCanvas::showSettingsMenu() {
        currentState = MenuState::SettingsMenu;
        if (mainMenuPanel) mainMenuPanel->setVisible(false);
        if (pauseMenuPanel) pauseMenuPanel->setVisible(false);
        if (settingsMenuPanel) settingsMenuPanel->setVisible(true);
    }

    void MenuCanvas::hideMenu() {
        currentState = MenuState::Hidden;
        if (mainMenuPanel) mainMenuPanel->setVisible(false);
        if (pauseMenuPanel) pauseMenuPanel->setVisible(false);
        if (settingsMenuPanel) settingsMenuPanel->setVisible(false);
    }

    void MenuCanvas::setupMainMenu() {
        mainMenuPanel = std::make_shared<UIPanel>();
        mainMenuPanel->setPosition(glm::vec2(canvasSize.x / 2 - 150.0f, canvasSize.y / 2 - 100.0f));
        mainMenuPanel->setSize(glm::vec2(300.0f, 200.0f));
        addWidget(mainMenuPanel);

        // 新游戏按钮
        auto newGameButton = std::make_shared<UIButton>("New Game");
        newGameButton->setPosition(glm::vec2(50.0f, 150.0f));
        newGameButton->setSize(glm::vec2(200.0f, 40.0f));
        newGameButton->setOnClick([this]() {
            if (onNewGame) onNewGame();
        });
        mainMenuPanel->addChild(newGameButton);

        // 加载游戏按钮
        auto loadGameButton = std::make_shared<UIButton>("Load Game");
        loadGameButton->setPosition(glm::vec2(50.0f, 100.0f));
        loadGameButton->setSize(glm::vec2(200.0f, 40.0f));
        loadGameButton->setOnClick([this]() {
            if (onLoadGame) onLoadGame();
        });
        mainMenuPanel->addChild(loadGameButton);

        // 设置按钮
        auto settingsButton = std::make_shared<UIButton>("Settings");
        settingsButton->setPosition(glm::vec2(50.0f, 50.0f));
        settingsButton->setSize(glm::vec2(200.0f, 40.0f));
        settingsButton->setOnClick([this]() {
            if (onSettings) onSettings();
        });
        mainMenuPanel->addChild(settingsButton);

        // 退出按钮
        auto quitButton = std::make_shared<UIButton>("Quit");
        quitButton->setPosition(glm::vec2(50.0f, 0.0f));
        quitButton->setSize(glm::vec2(200.0f, 40.0f));
        quitButton->setOnClick([this]() {
            if (onQuit) onQuit();
        });
        mainMenuPanel->addChild(quitButton);
    }

    void MenuCanvas::setupPauseMenu() {
        pauseMenuPanel = std::make_shared<UIPanel>();
        pauseMenuPanel->setPosition(glm::vec2(canvasSize.x / 2 - 100.0f, canvasSize.y / 2 - 75.0f));
        pauseMenuPanel->setSize(glm::vec2(200.0f, 150.0f));
        addWidget(pauseMenuPanel);

        // 继续游戏按钮
        auto resumeButton = std::make_shared<UIButton>("Resume");
        resumeButton->setPosition(glm::vec2(25.0f, 100.0f));
        resumeButton->setSize(glm::vec2(150.0f, 40.0f));
        resumeButton->setOnClick([this]() {
            hideMenu();
        });
        pauseMenuPanel->addChild(resumeButton);

        // 主菜单按钮
        auto mainMenuButton = std::make_shared<UIButton>("Main Menu");
        mainMenuButton->setPosition(glm::vec2(25.0f, 50.0f));
        mainMenuButton->setSize(glm::vec2(150.0f, 40.0f));
        mainMenuButton->setOnClick([this]() {
            showMainMenu();
        });
        pauseMenuPanel->addChild(mainMenuButton);

        // 退出按钮
        auto quitButton = std::make_shared<UIButton>("Quit");
        quitButton->setPosition(glm::vec2(25.0f, 0.0f));
        quitButton->setSize(glm::vec2(150.0f, 40.0f));
        quitButton->setOnClick([this]() {
            if (onQuit) onQuit();
        });
        pauseMenuPanel->addChild(quitButton);
    }

    void MenuCanvas::setupSettingsMenu() {
        settingsMenuPanel = std::make_shared<UIPanel>();
        settingsMenuPanel->setPosition(glm::vec2(canvasSize.x / 2 - 150.0f, canvasSize.y / 2 - 100.0f));
        settingsMenuPanel->setSize(glm::vec2(300.0f, 200.0f));
        addWidget(settingsMenuPanel);

        // 设置标题
        auto titleLabel = std::make_shared<UILabel>("Settings");
        titleLabel->setPosition(glm::vec2(100.0f, 160.0f));
        titleLabel->setSize(glm::vec2(100.0f, 30.0f));
        settingsMenuPanel->addChild(titleLabel);

        // 返回按钮
        auto backButton = std::make_shared<UIButton>("Back");
        backButton->setPosition(glm::vec2(50.0f, 20.0f));
        backButton->setSize(glm::vec2(200.0f, 40.0f));
        backButton->setOnClick([this]() {
            showMainMenu();
        });
        settingsMenuPanel->addChild(backButton);
    }

}