#pragma once

#include "UIManager.h"
#include "UICanvas.h"
#include "UIWidget.h"

namespace Cube {

    /**
     * UI系统使用示例和便利函数集合
     */
    class UIExample {
    public:
        /**
         * 创建一个简单的游戏HUD示例
         */
        static std::shared_ptr<HUDCanvas> createGameHUD() {
            auto hudCanvas = std::make_shared<HUDCanvas>();
            
            // 设置初始数值
            hudCanvas->setHealthBar(100.0f, 100.0f);
            hudCanvas->setScore(0);
            hudCanvas->setAmmo(30, 30);
            
            return hudCanvas;
        }

        /**
         * 创建一个主菜单示例
         */
        static std::shared_ptr<MenuCanvas> createMainMenu() {
            auto menuCanvas = std::make_shared<MenuCanvas>();
            
            // 设置菜单回调
            menuCanvas->setOnNewGame([]() {
                CUBE_LOG_INFO("New Game clicked!");
                // 这里可以调用游戏启动逻辑
            });
            
            menuCanvas->setOnLoadGame([]() {
                CUBE_LOG_INFO("Load Game clicked!");
                // 这里可以调用游戏加载逻辑
            });
            
            menuCanvas->setOnSettings([]() {
                CUBE_LOG_INFO("Settings clicked!");
                // 这里可以打开设置界面
            });
            
            menuCanvas->setOnQuit([]() {
                CUBE_LOG_INFO("Quit clicked!");
                // 这里可以调用退出游戏逻辑
            });
            
            return menuCanvas;
        }

        /**
         * 创建一个自定义对话框示例
         */
        static std::shared_ptr<UICanvas> createCustomDialog(const std::string& title, const std::string& message) {
            auto dialogCanvas = std::make_shared<UICanvas>();
            dialogCanvas->setSortOrder(300); // 对话框在最前面
            
            // 创建对话框背景
            auto dialogPanel = std::make_shared<UIPanel>();
            dialogPanel->setPosition(glm::vec2(400.0f, 300.0f));
            dialogPanel->setSize(glm::vec2(400.0f, 200.0f));
            dialogPanel->getStyle().backgroundColor = glm::vec4(0.8f, 0.8f, 0.8f, 0.95f);
            dialogPanel->getStyle().borderWidth = 3.0f;
            dialogPanel->getStyle().borderColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
            
            // 创建标题标签
            auto titleLabel = std::make_shared<UILabel>(title);
            titleLabel->setPosition(glm::vec2(20.0f, 160.0f));
            titleLabel->setSize(glm::vec2(360.0f, 30.0f));
            titleLabel->setTextColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
            dialogPanel->addChild(titleLabel);
            
            // 创建消息标签
            auto messageLabel = std::make_shared<UILabel>(message);
            messageLabel->setPosition(glm::vec2(20.0f, 100.0f));
            messageLabel->setSize(glm::vec2(360.0f, 50.0f));
            messageLabel->setTextColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
            dialogPanel->addChild(messageLabel);
            
            // 创建确定按钮
            auto okButton = std::make_shared<UIButton>("OK");
            okButton->setPosition(glm::vec2(280.0f, 20.0f));
            okButton->setSize(glm::vec2(100.0f, 40.0f));
            okButton->setOnClick([dialogCanvas]() {
                dialogCanvas->setVisible(false);
                CUBE_LOG_INFO("Dialog closed");
            });
            dialogPanel->addChild(okButton);
            
            dialogCanvas->addWidget(dialogPanel);
            return dialogCanvas;
        }

        /**
         * 创建一个简单的设置面板示例
         */
        static std::shared_ptr<UICanvas> createSettingsPanel() {
            auto settingsCanvas = std::make_shared<UICanvas>();
            settingsCanvas->setSortOrder(250);
            
            auto settingsPanel = std::make_shared<UIPanel>();
            settingsPanel->setPosition(glm::vec2(300.0f, 200.0f));
            settingsPanel->setSize(glm::vec2(500.0f, 400.0f));
            settingsPanel->getStyle().backgroundColor = glm::vec4(0.85f, 0.85f, 0.85f, 0.95f);
            settingsPanel->getStyle().borderWidth = 2.0f;
            
            // 标题
            auto titleLabel = std::make_shared<UILabel>("Game Settings");
            titleLabel->setPosition(glm::vec2(20.0f, 350.0f));
            titleLabel->setSize(glm::vec2(460.0f, 40.0f));
            settingsPanel->addChild(titleLabel);
            
            // 音量标签
            auto volumeLabel = std::make_shared<UILabel>("Master Volume");
            volumeLabel->setPosition(glm::vec2(20.0f, 300.0f));
            volumeLabel->setSize(glm::vec2(200.0f, 30.0f));
            settingsPanel->addChild(volumeLabel);
            
            // 分辨率标签
            auto resolutionLabel = std::make_shared<UILabel>("Resolution");
            resolutionLabel->setPosition(glm::vec2(20.0f, 250.0f));
            resolutionLabel->setSize(glm::vec2(200.0f, 30.0f));
            settingsPanel->addChild(resolutionLabel);
            
            // 应用按钮
            auto applyButton = std::make_shared<UIButton>("Apply");
            applyButton->setPosition(glm::vec2(280.0f, 50.0f));
            applyButton->setSize(glm::vec2(100.0f, 40.0f));
            applyButton->setOnClick([]() {
                CUBE_LOG_INFO("Settings applied!");
            });
            settingsPanel->addChild(applyButton);
            
            // 取消按钮
            auto cancelButton = std::make_shared<UIButton>("Cancel");
            cancelButton->setPosition(glm::vec2(390.0f, 50.0f));
            cancelButton->setSize(glm::vec2(100.0f, 40.0f));
            cancelButton->setOnClick([settingsCanvas]() {
                settingsCanvas->setVisible(false);
                CUBE_LOG_INFO("Settings cancelled");
            });
            settingsPanel->addChild(cancelButton);
            
            settingsCanvas->addWidget(settingsPanel);
            return settingsCanvas;
        }

        /**
         * 设置完整的UI系统示例
         */
        static void setupExampleUI() {
            UIManager& uiManager = UIManager::getInstance();
            
            // 创建游戏HUD
            auto hudCanvas = createGameHUD();
            uiManager.addCanvas(hudCanvas);
            
            // 创建主菜单
            auto menuCanvas = createMainMenu();
            menuCanvas->showMainMenu();
            uiManager.addCanvas(menuCanvas);
            
            CUBE_LOG_INFO("Example UI setup complete");
        }
    };

    /**
     * UI工厂类 - 用于创建常用的UI组件
     */
    class UIFactory {
    public:
        /**
         * 创建标准按钮
         */
        static std::shared_ptr<UIButton> createButton(const std::string& text, 
                                                     const glm::vec2& position, 
                                                     const glm::vec2& size,
                                                     std::function<void()> onClick = nullptr) {
            auto button = std::make_shared<UIButton>(text);
            button->setPosition(position);
            button->setSize(size);
            if (onClick) {
                button->setOnClick(onClick);
            }
            return button;
        }

        /**
         * 创建标准标签
         */
        static std::shared_ptr<UILabel> createLabel(const std::string& text,
                                                   const glm::vec2& position,
                                                   const glm::vec2& size,
                                                   const glm::vec4& textColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
            auto label = std::make_shared<UILabel>(text);
            label->setPosition(position);
            label->setSize(size);
            label->setTextColor(textColor);
            return label;
        }

        /**
         * 创建标准面板
         */
        static std::shared_ptr<UIPanel> createPanel(const glm::vec2& position,
                                                   const glm::vec2& size,
                                                   const glm::vec4& backgroundColor = glm::vec4(0.9f, 0.9f, 0.9f, 0.8f)) {
            auto panel = std::make_shared<UIPanel>();
            panel->setPosition(position);
            panel->setSize(size);
            panel->getStyle().backgroundColor = backgroundColor;
            return panel;
        }
    };

}