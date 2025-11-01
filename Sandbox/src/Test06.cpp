// test UI
#define TEST06
#ifdef TEST06

#include "Cube.h"

using namespace Cube;

class MyApp : public Application {
public:
    MyApp() : Application({1280, 720, "UI Test"}){
        std::shared_ptr<UILayer> uiLayer = std::make_shared<UILayer>();
        UIWidget::globalStyle.bgColor = {1.0f, 1.0f, 1.0f, 1.0f};
        // UIWidget::globalStyle.borderColor = {1.0f, 1.0f, 0.0f, 1.0f};
        // UIWidget::globalStyle.border = glm::vec4(2.0f);
        // std::shared_ptr<UIPanel> panel = std::make_shared<UIPanel>();
        // std::shared_ptr<UIImage> image = std::make_shared<UIImage>("assets/texture/test02.jpg");
        UILabel* label = new UILabel("你好世界Hello, World!", "../CubeEditor/assets/fonts/SourceHanSansSC/SourceHanSansSC-Normal.otf", 70, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec2(100, 100));
        // panel->addChild(image);
        // panel->setPosition({300.0f, 300.0f});
        // panel->setSize({400.0f, 300.0f});
        // uiLayer->addWidget(panel);
        UIButton* button = new UIButton( [] { CB_INFO("UIButton is clicked"); }, glm::vec2(200, 200), glm::vec2(100, 100));
        uiLayer->addWidget(label);
        uiLayer->addWidget(button);
        layers.pushOverLayer(uiLayer);
    }
};

int main() {
    MyApp app;
    app.run();
    return 0;
}

#endif
