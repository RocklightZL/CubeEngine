# Cube引擎游戏内UI系统

## 概述

这是为Cube游戏引擎设计的游戏内UI系统。该系统采用立即模式GUI的设计理念，提供了一个灵活且易于使用的UI框架，独立于游戏的ECS系统运行。

## 核心架构

### 主要组件

1. **UIManager** - UI管理器，作为Layer存在，负责管理所有UI画布
2. **UICanvas** - UI画布，管理一组相关的UI组件
3. **UIWidget** - UI组件基类，所有UI元素的基础
4. **UIButton** - 按钮组件
5. **UILabel** - 文本标签组件
6. **UIPanel** - 面板容器组件

### 专用画布

1. **HUDCanvas** - 游戏HUD画布（血量、分数、弹药等）
2. **MenuCanvas** - 菜单画布（主菜单、暂停菜单、设置菜单）

## 使用方法

### 1. 初始化UI系统

```cpp
#include "Cube.h"

// 在应用程序初始化时
Application* app = new Application();

// 创建UI管理器并添加到Layer栈
auto uiManager = std::make_shared<UIManager>();
app->pushLayer(uiManager.get());

// 设置屏幕尺寸
uiManager->setScreenSize(1920, 1080);
```

### 2. 创建游戏HUD

```cpp
// 创建HUD画布
auto hudCanvas = std::make_shared<HUDCanvas>();

// 设置HUD数据
hudCanvas->setHealthBar(100.0f, 100.0f);
hudCanvas->setScore(0);
hudCanvas->setAmmo(30, 30);

// 添加到UI管理器
UIManager::getInstance().addCanvas(hudCanvas);
```

### 3. 创建主菜单

```cpp
// 创建菜单画布
auto menuCanvas = std::make_shared<MenuCanvas>();

// 设置菜单回调
menuCanvas->setOnNewGame([]() {
    // 开始新游戏的逻辑
    CUBE_LOG_INFO("Starting new game...");
});

menuCanvas->setOnLoadGame([]() {
    // 加载游戏的逻辑
    CUBE_LOG_INFO("Loading game...");
});

menuCanvas->setOnQuit([]() {
    // 退出游戏的逻辑
    exit(0);
});

// 显示主菜单
menuCanvas->showMainMenu();

// 添加到UI管理器
UIManager::getInstance().addCanvas(menuCanvas);
```

### 4. 创建自定义UI

```cpp
// 创建自定义画布
auto customCanvas = std::make_shared<UICanvas>();

// 创建面板
auto panel = std::make_shared<UIPanel>();
panel->setPosition(glm::vec2(100.0f, 100.0f));
panel->setSize(glm::vec2(300.0f, 200.0f));

// 创建按钮
auto button = std::make_shared<UIButton>("Click Me!");
button->setPosition(glm::vec2(50.0f, 50.0f));
button->setSize(glm::vec2(200.0f, 40.0f));
button->setOnClick([]() {
    CUBE_LOG_INFO("Button clicked!");
});

// 创建标签
auto label = std::make_shared<UILabel>("Hello, World!");
label->setPosition(glm::vec2(50.0f, 100.0f));
label->setSize(glm::vec2(200.0f, 30.0f));

// 构建层次结构
panel->addChild(button);
panel->addChild(label);
customCanvas->addWidget(panel);

// 添加到UI管理器
UIManager::getInstance().addCanvas(customCanvas);
```

### 5. 处理事件

UI系统会自动处理鼠标和键盘事件。事件处理采用从前往后的顺序，优先级高的UI元素会先接收事件。

### 6. 更新UI数据

```cpp
// 在游戏循环中更新HUD
void updateGameHUD(float playerHealth, float maxHealth, int score, int ammo, int maxAmmo) {
    auto hudCanvas = getHUDCanvas(); // 获取HUD画布的引用
    hudCanvas->setHealthBar(playerHealth, maxHealth);
    hudCanvas->setScore(score);
    hudCanvas->setAmmo(ammo, maxAmmo);
}
```

## 渲染层级

UI系统支持多层级渲染，通过sortOrder控制渲染顺序：

- HUD: sortOrder = 100
- 菜单: sortOrder = 200
- 对话框: sortOrder = 300
- 数值越大，渲染越靠前

## 坐标系统

UI系统使用屏幕坐标系，左下角为(0,0)，右上角为(screenWidth, screenHeight)。

## 样式系统

每个UI组件都有一个Style结构，包含：
- backgroundColor: 背景颜色
- borderColor: 边框颜色
- borderWidth: 边框宽度
- padding: 内边距
- margin: 外边距

```cpp
// 自定义样式
auto button = std::make_shared<UIButton>("Styled Button");
button->getStyle().backgroundColor = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
button->getStyle().borderColor = glm::vec4(0.0f, 0.3f, 0.8f, 1.0f);
button->getStyle().borderWidth = 3.0f;
```

## 扩展指南

### 1. 创建新的UI组件

继承UIWidget类并实现必要的方法：

```cpp
class CustomWidget : public UIWidget {
public:
    CustomWidget();
    
    void render() override;
    bool onMousePressed(const glm::vec2& mousePos) override;
    
    // 自定义功能
    void setCustomProperty(int value) { customValue = value; }
    
private:
    int customValue = 0;
};
```

### 2. 创建新的画布类型

继承UICanvas类：

```cpp
class InventoryCanvas : public UICanvas {
public:
    InventoryCanvas();
    
    void addItem(const Item& item);
    void removeItem(int itemId);
    
private:
    std::vector<std::shared_ptr<UIWidget>> itemSlots;
    void setupInventoryGrid();
};
```

## 注意事项

1. UI系统独立于ECS，不要在ECS组件中直接操作UI
2. UI更新应该在主线程中进行
3. 避免在UI回调中进行重量级操作
4. 大量UI元素时注意性能优化

## TODO功能

1. 文本渲染系统集成
2. 纹理支持和九宫格拉伸
3. 动画系统
4. 拖拽功能
5. 输入框组件
6. 滚动视图组件
7. 布局管理器（自动布局）
8. 主题系统