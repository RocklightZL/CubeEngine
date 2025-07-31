// test data from CubeEditor
#define TEST05
#ifdef TEST05

#include "Cube/Renderer/FrameBuffer.h"
#include "Cube/Resource/ResourceManager.h"

#include <Cube.h>
#include <iostream>

using namespace Cube;

static Application* app = nullptr;
static int width = 1920;
static int height = 1080;
static float x = 0.0f;
static float y = 0.0f;
static float cameraX = 0.0f;
static float cameraY = 0.0f;
static Timer timer;

class App : public Application {
public:
    void run() override;
};

void App::run() {
    running = true;
    CB_CORE_INFO("Application run");

    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();

    FrameBuffer frameBuffer;
    frameBuffer.bindAttachment(width, height);

    while(running) {
        // º∆À„÷° ±º‰
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> frameDuration = currentTime - lastTime;
        lastTime = currentTime;
        float deltaTime = frameDuration.count();

        // CB_CORE_INFO("FPS: {}", 1/deltaTime);
        // std::cout << "FPS: " << 1 / deltaTime << "\n";

        Renderer2D::clearBuffer();

        for(Layer* layer : layers.getData()) {
            layer->onUpdate(deltaTime);
        }
        if(mainScene) {
            frameBuffer.bind();
            Renderer2D::setViewport(width, height);
            Renderer2D::clearBuffer();
            mainScene->onUpdate(deltaTime);
            FrameBuffer::bindDefaultFrameBuffer();
        }
        mainWindow->update();
    }
}

class TransformSystem : public System {
public:
    void onUpdate(Scene* scene, float deltaTime) override {
        processInput(deltaTime);
        // std::vector<Entity*> players = scene->getEntitiesWith<TransformComponent, SpriteComponent>();
        // for(auto p : players) {
        //     p->getComponent<TransformComponent>()->position = {x, y};
        // }
        std::vector<Entity*> cameras = scene->getEntitiesWith<TransformComponent, CameraComponent>();
        for(auto c : cameras) {
            c->getComponent<TransformComponent>()->position = {cameraX, cameraY};
            // c->getComponent<Cube::TransformComponent>()->scale = {cameraX + 1, cameraX + 1};
        }
    }

    std::string getName() const override {
        return "TransformSystem";
    }
private:
    void processInput(float deltaTime) {
        if(app->getWindow()->isKeyPressed(Key::Up)) {
            cameraY += 1000.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Key::Down)) {
            cameraY -= 1000.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Key::Right)) {
            cameraX += 1000.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Key::Left)) {
            cameraX -= 1000.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Key::D)) {
            x += 1000.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Key::A)) {
            x -= 1000.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Key::W)) {
            y += 1000.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Key::S)) {
            y -= 1000.0f * deltaTime;
        }
        if(app->getWindow()->isKeyPressed(Key::B)) {
            std::cout << "FPS: " << 1/deltaTime << "\n";
        }
    }

};

int main() {
    app = new Application({width, height, "Sandbox"});
    Renderer2D::init();
    Scene* scene = new Scene();
    SceneSerializer::deserialize(scene, "assets/scenes/TestScene02.scene");
    scene->setViewportSize({width, height});
    scene->addSystem(new RenderSystem());
    scene->addSystem(new TransformSystem());

    auto blocks = scene->getEntitiesWith<TransformComponent, SpriteComponent>();
    for(auto& b : blocks) {
        auto sc = b->getComponent<SpriteComponent>();
        sc->texture = new TextureAtlas("assets/texture/Directory.png");
    }

    app->setMainScene(scene);
    app->run();
    Renderer2D::shutdown();
    delete app;
    return 0;
}
#endif
