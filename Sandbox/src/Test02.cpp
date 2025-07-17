/*
#include <Cube.h>


Cube::Application* app;
float cameraX;
float cameraY = 1.0f;
float cameraR;
float cameraZoom = 1.0f;
float x = 100.0f;
float y = 100.0f;
float r;
float zoom = 1.0f;

class TestLayer2 : public Cube::Layer {
public:
    TestLayer2();
    void onUpdate() override;
    void processInput();

private:
    std::shared_ptr<Cube::Texture2D> texture;
    std::shared_ptr<Cube::Camera2D> camera;
};

TestLayer2::TestLayer2() {
    Cube::Renderer2D::init();
    texture = std::make_shared<Cube::Texture2D>("assets/texture/test03.jpg");
    camera = std::make_shared<Cube::Camera2D>(app->getWindow()->getPros().width, app->getWindow()->getPros().height);
}

void TestLayer2::onUpdate() {
    Cube::Renderer2D::setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    camera->setPosition({cameraX, cameraY});
    Cube::Renderer2D::beginFrame(camera->getPVMatrix());
    Cube::Renderer2D::drawQuad({x, y}, {100.0f, 100.0f}, texture, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f, {0.0f, 0.0f, 0.5f, 0.5f});
    // for(int i = 0; i < 50; i++) {
    //     for(int j = 0; j < 50; j++) {
    //         Cube::Renderer2D::drawQuad({x + 105 * i, y + 105 * j}, {100.0f, 100.0f}, texture, {1.0f, 1.0f, 1.0f, 1.0f}, 45.0f, {0.0f, 0.0f, 0.5f, 0.5f});
    //     }
    // }
    Cube::Renderer2D::endFrame();
    processInput();
}

void TestLayer2::processInput() {
    if(app->getWindow()->isKeyPressed(Cube::Key::Up)) {
        cameraY += 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::Down)) {
        cameraY -= 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::Right)) {
        cameraX += 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::Left)) {
        cameraX -= 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::D)) {
        x += 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::A)) {
        x -= 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::W)) {
        y += 1.0f;
    }
    if(app->getWindow()->isKeyPressed(Cube::Key::S)) {
        y -= 1.0f;
    }
}
*/